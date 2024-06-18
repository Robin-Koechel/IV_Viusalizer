#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <cstdint>
#include <chrono>
#include <utility>

#include <QString>
#include <QMap>
#include <QVector>
#include <QDateTime>

#include "TimeRange.h"
#include <RegionOfInterest.h>
#include <BoundingBox.h>
#include <cassandra.h>


namespace db {

using Iv_bb = iv_annotation::BoundingBox;
using BoundingBoxPtr = std::shared_ptr<Iv_bb>; // TODO: use unique? --> better performance but maybe not thread safe
using BoundingBoxList = QVector<BoundingBoxPtr>;
using BoundingBoxListPtr = std::shared_ptr<BoundingBoxList>;

using Iv_roi = iv_annotation::RegionOfInterest;
using RoiPtr = std::shared_ptr<Iv_roi>;
using RoiList = QVector<RoiPtr>;
using RoiListPtr = std::shared_ptr<RoiList>;

/**
 * @brief The DBManager class provides functionality to interact with a database for storing and retrieving annotations for images.
 */
class DBManager {

 private:
  static CassSession *session; /**< Cassandra session object (connection) - should be unique. */
  static CassUuidGen *uuidGen; /**< Cassandra UUID generator - should be unique. */
  static int ttl; /**< Time-to-live in seconds for entries in the database. */
  static std::map<int, std::shared_ptr<Iv_roi>> regionOfInterestCache;

  QString cameraUrl; /**< URL of the camera of this instance. */
  CassUuid cameraId; /**< UUID of the camera of this instance. */

  /**
   * @brief Fetches a region of interest entry from the database for a given ID.
   * @param roiId The ID of the region of interest to retrieve.
   * @return The retrieved region of interest.
   */
  static std::shared_ptr<Iv_roi> getRoiFromDb(int roiId);

  /**
   * @brief Parses bounding box annotations from a Cassandra value.
   * @param value The Cassandra value containing bounding box annotations.
   * @return The parsed list of bounding boxes.
   */
  static BoundingBoxList parseAnnotations(const CassValue *value);

  /**
   * @brief Parses roi id's from a Cassandra value.
   * @param value The Cassandra value containing roi list.
   * @return The parsed list id's.
   */
  static QVector<uint32_t> parseRoiIdList(const CassValue *value);

  /**
   * @brief Parses a single bounding box from a Cassandra value.
   * @param bbox_value The Cassandra value containing a single bounding box.
   * @return The parsed bounding box.
   */
  static BoundingBoxPtr parseBoundingBox(const CassValue *bbox_value);

  /**
   * @brief Parses coordinates from a Cassandra value.
   * @param field_value The Cassandra value containing coordinates.
   * @param coords Reference to the vector to store the parsed coordinates.
   */
  static void parseIntList(const CassValue *field_value, std::vector<int> &coords);

  /**
   * @brief Parses image size from a Cassandra value.
   * @param field_value The Cassandra value containing image size.
   * @param imageSize Reference to the cv::Size2i object to store the parsed image size.
   */
  static void parseImageSize(const CassValue *field_value, cv::Size2i &imageSize);

  /**
   * @brief Creates all necessary tables in the database.
   */
  static void createTables();

  /**
   * @brief Gets an empty Cassandra user-defined type for a bounding box.
   * @return The empty Cassandra user-defined type for bounding box.
   */
  static CassUserType *getEmptyCassBoundingBox();

  /**
   * @brief Prints errors related to this class to std::cerr.
   * @param cassandra_error The Cassandra error to handle.
   * @return True if there was no error, false otherwise.
   */
  static bool handleErrors(CassError cassandra_error);

  /**
   * @brief Prints errors related to this class to std::cerr.
   * @param future The Cassandra future to handle.
   * @return True if there was no error, false otherwise.
   */
  static bool handleErrors(CassFuture *future);

  /**
   * @brief Retrieves string value from a Cassandra row.
   * @param row The Cassandra row containing the string value.
   * @param columnName The name of the column containing the string value.
   * @return The retrieved string value.
   */
  static QString getStringFromRow(const CassRow *row, const char *columnName);

  /**
   * @brief Retrieves polygon value from a Cassandra row.
   * @param row The Cassandra row containing the polygon value.
   * @param columnName The name of the column containing the polygon value.
   * @return The retrieved polygon value.
   */
  static QPolygon getPolygonFromRow(const CassRow *row, const char *columnName);

  /**
   * @brief Retrieves boolean value from a Cassandra row.
   * @param row The Cassandra row containing the boolean value.
   * @param columnName The name of the column containing the boolean value.
   * @return The retrieved boolean value.
   */
  static bool getBoolFromRow(const CassRow *row, const char *columnName);

  /**
   * @brief Retrieves float value from a Cassandra row.
   * @param row The Cassandra row containing the float value.
   * @param columnName The name of the column containing the float value.
   * @return The retrieved float value.
   */
  static float getFloatFromRow(const CassRow *row, const char *columnName);

  /**
   * @brief Loads the camera UUID for this instance from the database.
   * @return The loaded camera UUID.
   */
  CassUuid loadCameraId();


 public:
  /**
   * @brief Constructs a DBManager object with the given camera URL.
   * @param url The URL of the camera.
   */
  explicit DBManager(QString url);

  /**
   * @brief Destructor. Closes the connection to the database and clears the regionOfInterestCache.
   */
  ~DBManager();

  /**
   * @brief Executes a query without parameters on the database.
   * @param query The query to execute.
   * @return True if the query is executed successfully, false otherwise.
   */
  static bool executeStaticQuery(const char *query);

  /**
   * @brief Checks if the DBManager is connected to the database.
   * @return True if connected, false otherwise.
   */
  static bool isConnected();

  /**
   * @brief Inserts a region of interest into the database.
   * @param roi The region of interest to insert.
   * @return True if the region of interest is inserted successfully, false otherwise.
   */
  static bool insertROI(const Iv_roi &roi);

  /**
   * @brief Gets timestamps of oldest and newest entry in the database
   * @return Tuple with timestamps
   */
  std::tuple<cass_int64_t, cass_int64_t, float> getTimeRangeTuple();

  /**
   * @brief Gets TimeRange of oldest to newest timestamp in the database
   * @return TimeRange of oldest to newest timestamp in the database
   */
  TimeRange getTimeRange();

  /**
   * @brief Retrieves a region of interest from the database or the regionOfInterestCache.
   * @param roiId The ID of the region of interest to retrieve.
   * @return The retrieved region of interest.
   */
  static std::shared_ptr<Iv_roi> getRegionOfInterest(int roiId);

  /**
   * @brief Inserts bounding boxes into the database for a given timestamp.
   * @param boundingBoxes The list of bounding boxes to insert.
   * @param roiList List with the ROIs with which the entry is linked to.
   * @param timestamp The timestamp (ns) associated with the bounding boxes.
   * @return True if the bounding boxes are inserted successfully, false otherwise.
   */
  bool insertBoundingBoxes(const QVector<std::shared_ptr<iv_annotation::BoundingBox>>& boundingBoxes,
                                      const QVector<std::shared_ptr<iv_annotation::RegionOfInterest>>& roiList,
                                      float fps,
                                      uint64_t timestamp);

  std::tuple<QVector<BoundingBoxList>, QVector<RoiList>, float> getBoundingBoxes(uint64_t start, uint64_t end);

};
}  // namespace db

#endif  // DBMANAGER_H
