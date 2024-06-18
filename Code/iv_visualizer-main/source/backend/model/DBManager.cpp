#include "DBManager.h"

namespace db {


    CassSession *DBManager::session = nullptr;
    CassUuidGen *DBManager::uuidGen = nullptr;
    int DBManager::ttl = 1209600;
    std::map<int, std::shared_ptr<Iv_roi>> DBManager::regionOfInterestCache;


    DBManager::DBManager(QString cameraUrl) {

        this->cameraUrl = std::move(cameraUrl);

        // DataStax C/C++ Driver suggests to use as few instances as possible, so I made it static
        if (session == nullptr) {
            cass_log_set_level(CASS_LOG_ERROR);

            session = cass_session_new();
            uuidGen = cass_uuid_gen_new();

            const char *scyllaHost = std::getenv("SCYLLA_IP");
            if (scyllaHost == nullptr) {
                scyllaHost = "scylla";
            }

            char *ttlString = std::getenv("TTL");
            if (ttlString != nullptr) {
                ttl = std::stoi(ttlString);
            }

            CassCluster *cluster = cass_cluster_new();
            cass_cluster_set_contact_points(cluster, scyllaHost);

            const char *username = std::getenv("SCYLLA_USERNAME");
            const char *password = std::getenv("SCYLLA_PASSWORD");
            if (username != nullptr && password != nullptr) {
                cass_cluster_set_credentials(cluster, username, password);
            }

            CassFuture *connectionFuture = nullptr;
            CassError cassError = CASS_OK;

            while (true) {
                connectionFuture = cass_session_connect(session, cluster);
                cassError = cass_future_error_code(connectionFuture);
                if (handleErrors(cassError)) {
                  break;
                } else {
                  cass_future_free(connectionFuture);
                  std::cerr << "Retrying to connect to ScyllaDB in 5 seconds...\n";
                  sleep(5);
                }
            }
            cass_cluster_free(cluster);
            cass_future_free(connectionFuture);
        }

        createTables();
        this->cameraId = loadCameraId();
    }


    DBManager::~DBManager() {
    }


    bool DBManager::isConnected() {
        const char *query = "SELECT * FROM system.local";
        return executeStaticQuery(query);
    }


    bool DBManager::insertROI(const Iv_roi &roi) {
        const char *query =
                "INSERT INTO visualizer.ROI"
                "(id, points, name, interestInside, color, opacity) "
                "VALUES (?, ?, ?, ?, ?, ?);";

        CassStatement *statement = cass_statement_new(query, 6);

        // id
        cass_statement_bind_int32(statement, 0, roi.get_id());

        // points (polygon)
        const QPolygon &polygon = roi.get_polygon();
        CassCollection *points =
                cass_collection_new(CASS_COLLECTION_TYPE_LIST, polygon.size());

        for (const QPoint &currentPoint: polygon) {
            CassTuple *pointTuple = cass_tuple_new(2);
            cass_tuple_set_int32(pointTuple, 0, currentPoint.x());
            cass_tuple_set_int32(pointTuple, 1, currentPoint.y());

            cass_collection_append_tuple(points, pointTuple);
            cass_tuple_free(pointTuple);
        }
        cass_statement_bind_collection(statement, 1, points);

        // name
        std::string nameStr = roi.get_name().toStdString();
        cass_statement_bind_string(statement, 2, nameStr.c_str());

        // interestInside
        cass_bool_t getIntersectsInside = roi.get_is_inside() ? cass_true : cass_false;
        cass_statement_bind_bool(statement, 3, getIntersectsInside);

        // color
        std::string colorStr = roi.get_color().name().toStdString();
        cass_statement_bind_string(statement, 4, colorStr.c_str());

        // opacity
        float opacity = roi.get_opacity();
        cass_statement_bind_float(statement, 5, opacity);


        // execute
        CassFuture *result_future = cass_session_execute(session, statement);
        CassError cassandraError = cass_future_error_code(result_future);

        cass_future_free(result_future);
        cass_statement_free(statement);
        cass_collection_free(points);

        return handleErrors(cassandraError);
    }

    bool DBManager::insertBoundingBoxes(const QVector<std::shared_ptr<iv_annotation::BoundingBox>>& boundingBoxes,
                                        const QVector<std::shared_ptr<iv_annotation::RegionOfInterest>>& roiList,
                                        float fps,
                                        uint64_t timestamp) {

      // TODO: not efficient and not really necessary
      for (const auto& roi : roiList) {
        insertROI(*roi);
      }

      const char* query =
          "INSERT INTO visualizer.Annotation "
          "(camera, date, timestamp, fps, annotation, roi) "
          "VALUES (?, ?, ?, ?, ?, ?) USING TTL ?;";

      CassStatement* statement = cass_statement_new(query, 7);

      // camera
      cass_statement_bind_uuid(statement, 0, this->cameraId);

      // date
      cass_uint32_t date = cass_date_from_epoch(timestamp / 1000000000);
      cass_statement_bind_uint32(statement, 1, date);

      // timestamp
      cass_statement_bind_int64(statement, 2, timestamp);

      // fps
      cass_statement_bind_float(statement, 3, fps);

      // annotationList
      CassCollection* annotationList = cass_collection_new(CASS_COLLECTION_TYPE_LIST, boundingBoxes.size());

      for (const auto& boundingBox : boundingBoxes) {
        CassUserType* cassBoundingBox = getEmptyCassBoundingBox();

        // extract data from bounding box
        auto coords = boundingBox->get_coords(Iv_bb::Format::TLWH);
        auto confidence = boundingBox->get_confidence();
        auto label = boundingBox->get_label();
        auto imageSize = boundingBox->get_image_wh();
        auto format = Iv_bb::Format::TLWH;
        auto rotation = boundingBox->get_rotation();

        // coords
        CassCollection* cassCoords =
            cass_collection_new(CASS_COLLECTION_TYPE_LIST, coords.size());
        for (auto coord : coords) {
          cass_collection_append_int32(cassCoords, coord);
        }
        cass_user_type_set_collection(cassBoundingBox, 0, cassCoords);
        cass_collection_free(cassCoords);

        // confidence
        cass_user_type_set_float(cassBoundingBox, 1, confidence);

        // label
        auto labelStr = label.toStdString();
        cass_user_type_set_string(cassBoundingBox, 2, labelStr.c_str());

        // imagesize
        CassTuple* cassImagesize = cass_tuple_new(2);
        cass_tuple_set_int32(cassImagesize, 0, imageSize.height);
        cass_tuple_set_int32(cassImagesize, 1, imageSize.width);
        cass_user_type_set_tuple(cassBoundingBox, 3, cassImagesize);
        cass_tuple_free(cassImagesize);

        // format
        cass_user_type_set_int32(cassBoundingBox, 4, static_cast<int>(format));

        // rotation
        cass_user_type_set_int32(cassBoundingBox, 5, rotation);

        cass_collection_append_user_type(annotationList, cassBoundingBox);
        cass_user_type_free(cassBoundingBox);
      }

      cass_statement_bind_collection(statement, 4, annotationList);

      // roi
      CassCollection* cassRoiList =
          cass_collection_new(CASS_COLLECTION_TYPE_LIST, roiList.size());

      for (const auto& roi : roiList) {
        cass_collection_append_int32(cassRoiList, roi->get_id());
      }
      cass_statement_bind_collection(statement, 5, cassRoiList);

      // ttl
      cass_statement_bind_int32(statement, 6, ttl);

      // execute
      CassFuture* queryFuture = cass_session_execute(session, statement);
      CassError cassandraError = cass_future_error_code(queryFuture);

      cass_statement_free(statement);
      cass_future_free(queryFuture);
      cass_collection_free(annotationList);
      return handleErrors(cassandraError);
    }


    std::tuple<QVector<BoundingBoxList>, QVector<RoiList>, float> DBManager::getBoundingBoxes(uint64_t start, uint64_t end) {
      const char *query =
          "SELECT annotation, roi, fps, timestamp "
          "FROM visualizer.Annotation "
          "WHERE camera = ? "
          "AND timestamp >= ? "
          "AND timestamp <= ? "
          "ALLOW FILTERING;";

      // binds
      CassStatement *statement = cass_statement_new(query, 3);
      cass_statement_bind_uuid(statement, 0, this->cameraId);
      cass_statement_bind_int64(statement, 1, start);
      cass_statement_bind_int64(statement, 2, end);
      CassFuture *queryFuture = cass_session_execute(session, statement);

      QVector<BoundingBoxList> boundingBoxesOverTime;
      QVector<RoiList> roisOverTime;
      float fps;

      if (!handleErrors(queryFuture)) {
        return std::make_tuple(boundingBoxesOverTime, roisOverTime, 30);
      }

      const CassResult *result = cass_future_get_result(queryFuture);
      CassIterator *iterator = cass_iterator_from_result(result);

      // get rows within timestamps
      while (cass_iterator_next(iterator)) {

        const CassRow *row = cass_iterator_get_row(iterator);

        // timestamp
        cass_int64_t cassTimestamp = 0;
        const CassValue *timestampVal = cass_row_get_column_by_name(row, "timestamp");
        cass_value_get_int64(timestampVal, &cassTimestamp);
        uint64_t timestamp = static_cast<uint64_t>(cassTimestamp);

        // fps
        cass_float_t cassFps = 0;
        const CassValue *fpsVal = cass_row_get_column_by_name(row, "fps");
        cass_value_get_float(fpsVal, &cassFps);
        fps = static_cast<float>(cassFps);

        // annotation
        const CassValue *annotationVal = cass_row_get_column_by_name(row, "annotation");
        BoundingBoxList boxListFrame = parseAnnotations(annotationVal);
        boundingBoxesOverTime.push_back(boxListFrame);

        // roi
        const CassValue *roiVal = cass_row_get_column_by_name(row, "roi");
        std::vector<int> roiIdList;
        parseIntList(roiVal, roiIdList);

        RoiList roiListFrame = RoiList();
        for (const auto roiId: roiIdList) {
          RoiPtr roi = getRegionOfInterest(roiId);
          roiListFrame.append(roi);
        }
        roisOverTime.push_back(std::move(roiListFrame));
      }

      cass_result_free(result);
      cass_iterator_free(iterator);

      cass_statement_free(statement);
      cass_future_free(queryFuture);

      return std::make_tuple(std::move(boundingBoxesOverTime), std::move(roisOverTime), fps);
    }


    std::tuple<cass_int64_t, cass_int64_t, float> DBManager::getTimeRangeTuple() {
      const char *query = "SELECT MIN(timestamp) AS min_timestamp, MAX(timestamp) AS max_timestamp, fps "
                          "FROM visualizer.Annotation;";

      CassStatement *statement = cass_statement_new(query, 0);
      CassFuture *future = cass_session_execute(session, statement);
      const CassResult *result = cass_future_get_result(future);

      cass_int64_t minTimestamp = 0;
      cass_int64_t maxTimestamp = 0;
      float fps = 0;

      if (result != nullptr) {
        const CassRow *row = cass_result_first_row(result);

        const CassValue *minTimestampValue = cass_row_get_column_by_name(row, "min_timestamp");
        const CassValue *maxTimestampValue = cass_row_get_column_by_name(row, "max_timestamp");
        const CassValue *fpsValue = cass_row_get_column_by_name(row, "fps");

        if (minTimestampValue != nullptr) {
          cass_value_get_int64(minTimestampValue, &minTimestamp);
        }
        if (maxTimestampValue != nullptr) {
          cass_value_get_int64(maxTimestampValue, &maxTimestamp);
        }
        if (fpsValue != nullptr) {
          cass_value_get_float(fpsValue, &fps);
        }
        cass_result_free(result);
      }

      cass_future_free(future);
      cass_statement_free(statement);

      return std::make_tuple(minTimestamp, maxTimestamp, fps);
    }

    TimeRange DBManager::getTimeRange() {
      auto [cass_oldest, cass_newest, fps] = getTimeRangeTuple();
      return TimeRange(cass_oldest, cass_newest, fps);
    }


    RoiPtr DBManager::getRegionOfInterest(int roiId) {

      if (regionOfInterestCache.contains(roiId)) {
        return regionOfInterestCache[roiId];
      } else {
        RoiPtr roi = getRoiFromDb(roiId);
        regionOfInterestCache[roiId] = roi;
        return roi;
      }
    }


    RoiPtr DBManager::getRoiFromDb(int roiId) {
        const char *query =
                "SELECT name, points, interestInside, color, opacity "
                "FROM visualizer.ROI "
                "WHERE id = ?";

        CassStatement *statement = cass_statement_new(query, 1);
        cass_statement_bind_int32(statement, 0, roiId);

        CassFuture *future = cass_session_execute(session, statement);

        const CassResult *result = cass_future_get_result(future);
        RoiPtr roi(new Iv_roi());

        if (result != nullptr) {
            const CassRow *row = cass_result_first_row(result);

            auto name = getStringFromRow(row, "name");
            auto polygon = getPolygonFromRow(row, "points");
            auto isInside = getBoolFromRow(row, "interestInside");
            auto opacity = getFloatFromRow(row, "opacity");

            roi->set_polygon(polygon);
            roi->set_name(name);
            roi->set_id(roiId);
            roi->set_is_inside(isInside);
            roi->set_opacity(opacity);

            cass_result_free(result);
        }
        cass_future_free(future);
        cass_statement_free(statement);

        return roi;
    }


    QString DBManager::getStringFromRow(const CassRow *row, const char *columnName) {
        const CassValue *value = cass_row_get_column_by_name(row, columnName);
        const char *str = "";
        size_t strLength = 0;
        cass_value_get_string(value, &str, &strLength);
        return QString::fromUtf8(str, static_cast<int>(strLength));
    }


    QPolygon DBManager::getPolygonFromRow(const CassRow *row, const char *columnName) {
        QPolygon polygon;
        const CassValue *pointsValue = cass_row_get_column_by_name(row, columnName);

        if (pointsValue == nullptr) {
            return polygon;
        }

        CassIterator *pointsIterator = cass_iterator_from_collection(pointsValue);
        if (pointsIterator == nullptr) {
            return polygon;
        }

        while (cass_iterator_next(pointsIterator)) {
            const CassValue *pointValue = cass_iterator_get_value(pointsIterator);
            if (pointValue == nullptr) {
                continue;
            }
            CassIterator *tupleIterator = cass_iterator_from_tuple(pointValue);
            if (tupleIterator == nullptr) {
                continue;
            }

            int xCoord = 0;
            int yCoord = 0;

            if (cass_iterator_next(tupleIterator)) {
                const CassValue *xValue = cass_iterator_get_value(tupleIterator);
                if (xValue != nullptr) {
                    cass_value_get_int32(xValue, &xCoord);
                }
            }

            if (cass_iterator_next(tupleIterator)) {
                const CassValue *yValue = cass_iterator_get_value(tupleIterator);
                if (yValue != nullptr) {
                    cass_value_get_int32(yValue, &yCoord);
                }
            }
            polygon << QPoint(xCoord, yCoord);

            cass_iterator_free(tupleIterator);
        }

        cass_iterator_free(pointsIterator);

        return polygon;
    }


    bool DBManager::getBoolFromRow(const CassRow *row, const char *columnName) {
        const CassValue *value = cass_row_get_column_by_name(row, columnName);
        cass_bool_t boolValue = cass_true;
        cass_value_get_bool(value, &boolValue);
        return boolValue == cass_true;
    }


    float DBManager::getFloatFromRow(const CassRow *row, const char *columnName) {
        const CassValue *value = cass_row_get_column_by_name(row, columnName);
        float floatValue = 1.f;
        cass_value_get_float(value, &floatValue);
        return floatValue;
    }


    BoundingBoxList DBManager::parseAnnotations(const CassValue *value) {
      auto boxList = BoundingBoxList();

      if (value == nullptr) {
        return boxList;
      }

      CassIterator *list_iterator = cass_iterator_from_collection(value);
      if (list_iterator == nullptr) {
        return boxList;
      }

      while (cass_iterator_next(list_iterator)) {
        const CassValue *bbox_value = cass_iterator_get_value(list_iterator);

        if (bbox_value != nullptr) {
          BoundingBoxPtr bbox = parseBoundingBox(bbox_value);
          if (bbox != nullptr) {
            boxList.push_back(std::move(bbox)); // Use std::move here
          }
        }
      }

      cass_iterator_free(list_iterator);
      return boxList;
    }


    QVector<uint32_t> DBManager::parseRoiIdList(const CassValue *value) {
        QVector<uint32_t> roiList;
        CassIterator *list_iterator = cass_iterator_from_collection(value);

        while (cass_iterator_next(list_iterator)) {
            const CassValue *roi_value = cass_iterator_get_value(list_iterator);
            cass_int32_t roi_id;
            cass_value_get_int32(roi_value, &roi_id);

            roiList.push_back(static_cast<uint32_t>(roi_id));
        }

        cass_iterator_free(list_iterator);
        return roiList;
    }


    BoundingBoxPtr DBManager::parseBoundingBox(const CassValue *bbox_value) {

      CassIterator *fields_iterator = cass_iterator_fields_from_user_type(bbox_value);

      std::vector<int> coords;
      float confidence = 1.f;
      QString label;
      cv::Size2i imageSize;
      int format = 0;
      int rotation = 0;

      while (cass_iterator_next(fields_iterator)) {
        const char *field_name = "";
        size_t field_name_length = 0;
        const CassValue *field_value = cass_iterator_get_user_type_field_value(fields_iterator);
        cass_iterator_get_user_type_field_name(fields_iterator, &field_name, &field_name_length);
        std::string field_name_str(field_name, field_name_length);

        if (field_name_str == "coords") {
          parseIntList(field_value, coords);

        } else if (field_name_str == "confidence") {
          cass_value_get_float(field_value, &confidence);

        } else if (field_name_str == "label") {
          const char *label_c_str = "";
          size_t label_length = 0;
          cass_value_get_string(field_value, &label_c_str, &label_length);
          label = QString::fromUtf8(label_c_str, label_length);

        } else if (field_name_str == "imagesize") {
          parseImageSize(field_value, imageSize);

        } else if (field_name_str == "format") {
          cass_value_get_int32(field_value, &format);

        } else if (field_name_str == "rotation") {
          cass_value_get_int32(field_value, &rotation);
        }
      }

      cass_iterator_free(fields_iterator);

      return std::make_shared<Iv_bb>(coords, confidence, label, imageSize, static_cast<Iv_bb::Format>(format), rotation);
    }


    void DBManager::parseIntList(const CassValue *field_value, std::vector<int> &coords) {

        if (field_value == nullptr) {
            return;
        }

        CassIterator *coords_iterator = cass_iterator_from_collection(field_value);
        if (!coords_iterator) {
            return;
        }

        while (cass_iterator_next(coords_iterator)) {
            const CassValue *coord_value = cass_iterator_get_value(coords_iterator);

            if (coord_value == nullptr || cass_value_is_null(coord_value)) {
                continue;
            }

            cass_int32_t coord = 0;
            CassError cassError = cass_value_get_int32(coord_value, &coord);
            handleErrors(cassError);

            coords.push_back(coord);
        }

        cass_iterator_free(coords_iterator);
    }


    void DBManager::parseImageSize(const CassValue *field_value, cv::Size2i &imageSize) {
        CassIterator *tuple_iterator = cass_iterator_from_tuple(field_value);
        if (cass_iterator_next(tuple_iterator)) {
            const CassValue *width_value = cass_iterator_get_value(tuple_iterator);
            cass_int32_t height = 0;
            cass_value_get_int32(width_value, &height);

            if (cass_iterator_next(tuple_iterator)) {
                const CassValue *height_value = cass_iterator_get_value(tuple_iterator);
                cass_int32_t width = 0;
                cass_value_get_int32(height_value, &width);

                imageSize = cv::Size2i(width, height);
            }
        }
        cass_iterator_free(tuple_iterator);
    }


    CassUuid DBManager::loadCameraId() {
        const char *query =
                "SELECT id FROM visualizer.Camera "
                "WHERE url = ? "
                "ALLOW FILTERING;";

        CassStatement *statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, this->cameraUrl.toStdString().c_str());

        CassFuture *resultFuture = cass_session_execute(session, statement);
        handleErrors(resultFuture);
        CassUuid uuid;

        const CassResult *result = cass_future_get_result(resultFuture);
        if (result != nullptr) {
            CassIterator *iterator = cass_iterator_from_result(result);

            if (cass_iterator_next(iterator)) {
                const CassRow *row = cass_iterator_get_row(iterator);
                cass_value_get_uuid(cass_row_get_column(row, 0), &uuid);
            } else {
                const char *insertQuery =
                        "INSERT INTO visualizer.Camera (id, url)"
                        "VALUES (?, ?)";

                CassStatement *insertStatement = cass_statement_new(insertQuery, 2);

                cass_uuid_gen_random(uuidGen, &uuid);

                cass_statement_bind_uuid(insertStatement, 0, uuid);
                cass_statement_bind_string(
                        insertStatement, 1, this->cameraUrl.toStdString().c_str());

                CassFuture *insertFuture = cass_session_execute(session, insertStatement);
                handleErrors(insertFuture);

                cass_statement_free(insertStatement);
                cass_future_free(insertFuture);
            }

            cass_iterator_free(iterator);
            cass_result_free(result);
        }

        cass_statement_free(statement);
        cass_future_free(resultFuture);

        return uuid;
    }


    void DBManager::createTables() {

        executeStaticQuery(
                "CREATE KEYSPACE IF NOT EXISTS visualizer "
                "WITH replication = {"
                "'class': 'SimpleStrategy', 'replication_factor': 1"  // throws annoying warning
                "};");

        executeStaticQuery(
                "CREATE TYPE IF NOT EXISTS visualizer.BoundingBox ("
                "coords LIST<INT>, "
                "confidence FLOAT, "
                "label TEXT, "
                "imagesize TUPLE<INT, INT>, "
                "format INT, "
                "rotation INT"
                ");");

        executeStaticQuery(
                "CREATE TABLE IF NOT EXISTS visualizer.Annotation ("
                "camera UUID, "
                "date DATE, "
                "timestamp BIGINT, "
                "fps FLOAT, "
                "annotation LIST<FROZEN<visualizer.BoundingBox>>, "
                "roi LIST<INT>, "
                "PRIMARY KEY ((camera, date), timestamp)"
                ");");

        executeStaticQuery(
                "CREATE TABLE IF NOT EXISTS visualizer.ROI ("
                "id INT PRIMARY KEY, "
                "points LIST<FROZEN<TUPLE<INT, INT>>>, "
                "name TEXT, "
                "interestInside BOOLEAN, "
                "color TEXT, "
                "opacity FLOAT"
                ");");

        executeStaticQuery(
                "CREATE TABLE IF NOT EXISTS visualizer.Camera ("
                "id UUID PRIMARY KEY, "
                "url TEXT"
                ");");
    }


    CassUserType *DBManager::getEmptyCassBoundingBox() {
        CassDataType *boundingBox = cass_data_type_new_udt(6);
        cass_data_type_add_sub_value_type_by_name(boundingBox, "coords", CASS_VALUE_TYPE_LIST);
        cass_data_type_add_sub_value_type_by_name(boundingBox, "confidence", CASS_VALUE_TYPE_FLOAT);
        cass_data_type_add_sub_value_type_by_name(boundingBox, "label", CASS_VALUE_TYPE_TEXT);
        cass_data_type_add_sub_value_type_by_name(boundingBox, "imagesize", CASS_VALUE_TYPE_TUPLE);
        cass_data_type_add_sub_value_type_by_name(boundingBox, "format", CASS_VALUE_TYPE_INT);
        cass_data_type_add_sub_value_type_by_name(boundingBox, "rotation", CASS_VALUE_TYPE_INT);
        return cass_user_type_new_from_data_type(boundingBox);
    }


    bool DBManager::executeStaticQuery(const char *query) {
        CassStatement *statement = cass_statement_new(query, 0);
        CassFuture *queryFuture = cass_session_execute(session, statement);
        CassError cassandraError = cass_future_error_code(queryFuture);

        cass_statement_free(statement);
        cass_future_free(queryFuture);

        return handleErrors(cassandraError);
    }


    bool DBManager::handleErrors(CassError cassandraError) {
        if (cassandraError != CASS_OK) {
            const char *errorMessage = cass_error_desc(cassandraError);
            std::cerr << "Error: " << errorMessage << '\n';
            return false;
        }
        return true;
    }


    bool DBManager::handleErrors(CassFuture *future) {
        CassError cassandraError = cass_future_error_code(future);
        return handleErrors(cassandraError);
    }

    }  // namespace db

