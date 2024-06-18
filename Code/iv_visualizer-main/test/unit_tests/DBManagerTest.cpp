#include <QRandomGenerator>
#include <chrono>

#include "DBManager.h"
#include "gtest/gtest.h"

// (not our code) iv_annotation::BoundingBox and RegionOfInterest allow a standard constructor which leads to invalid attributes. This must be corrected in iv_annotation!

class DBManagerTest : public ::testing::Test {
 protected:
  db::DBManager* dbManager;

  void SetUp() override {
    dbManager = new db::DBManager("test_db");
    ASSERT_TRUE(dbManager->isConnected());
  }

  void TearDown() override {
    db::DBManager::executeStaticQuery("DROP KEYSPACE visualizer;");
    delete dbManager;
    dbManager = nullptr;
  }
};

// RegionOfInterest
iv_annotation::RegionOfInterest roiRandom() {
  QString name = "Random_" + QString::number(QRandomGenerator::global()->generate());
  int id = QRandomGenerator::global()->generate();
  QPolygon polygon;
  for (int i = 0; i < 5; ++i) {
    polygon << QPoint(QRandomGenerator::global()->bounded(2000),
                      QRandomGenerator::global()->bounded(2000));
  }
  bool isInside = QRandomGenerator::global()->bounded(2); // 0 or 1
  float opacity = QRandomGenerator::global()->bounded(101) / 100.0f; // 0.00 to 1.00

  return iv_annotation::RegionOfInterest(name, id, polygon, isInside, opacity);
}

iv_annotation::RegionOfInterest roiZero() {
  QString name = "";
  int id = 0;
  QPolygon polygon(1, QPoint(0, 0));
  bool isInside = false;
  float opacity = 0.0f;

  return iv_annotation::RegionOfInterest(name, id, polygon, isInside, opacity);
}

iv_annotation::RegionOfInterest roiMax() {
  QString name = "Max_Value";
  int id = std::numeric_limits<int>::max();
  QPolygon polygon;
  polygon << QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
  bool isInside = true;
  float opacity = 1.0f;

  return iv_annotation::RegionOfInterest(name, id, polygon, isInside, opacity);
}

// BoundingBox
iv_annotation::BoundingBox* bboxRandom() {
    cv::Size2i imageSize(4000, 4000);

    int width = QRandomGenerator::global()->bounded(1, imageSize.width);
    int height = QRandomGenerator::global()->bounded(1, imageSize.height);

    int x = QRandomGenerator::global()->bounded(0, imageSize.width - width);
    int y = QRandomGenerator::global()->bounded(0, imageSize.height - height);

    std::vector<int> coords = {x, y, width, height};
    float confidence = QRandomGenerator::global()->generateDouble();
    QString label = "RandomLabel_" + QString::number(QRandomGenerator::global()->generate());
    iv_annotation::BoundingBox::Format format = iv_annotation::BoundingBox::Format::TLWH;
    int rotation = QRandomGenerator::global()->bounded(0, 360);

    return new iv_annotation::BoundingBox(coords, confidence, label, imageSize, format, rotation);
}


iv_annotation::BoundingBox *bboxZero() {
  std::vector<int> coords = {0, 0, 1, 1};
  float confidence = 0.0f;
  QString label;
  cv::Size2i imageSize(1, 1);
  iv_annotation::BoundingBox::Format format = iv_annotation::BoundingBox::Format::TLWH;
  int rotation = 0;

  return new iv_annotation::BoundingBox(coords, confidence, label, imageSize, format, rotation);
}

iv_annotation::BoundingBox *bboxMax() {
  int max_coord_value = std::numeric_limits<int>::max();
  std::vector<int> coords = {0, 0, max_coord_value, max_coord_value};
  float confidence = 1.0f;
  QString label = "MaxLabel";
  cv::Size2i imageSize(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
  iv_annotation::BoundingBox::Format format = iv_annotation::BoundingBox::Format::TLWH;
  int rotation = 359;

  return new iv_annotation::BoundingBox(coords, confidence, label, imageSize, format, rotation);
}


// RegionOfInterest
TEST_F(DBManagerTest, Random_InsertAndGetROI) {

  iv_annotation::RegionOfInterest roi = roiRandom();
  ASSERT_TRUE(db::DBManager::insertROI(roi));

  auto retrievedRoi = db::DBManager::getRegionOfInterest(roi.get_id());

  EXPECT_EQ(retrievedRoi->get_id(), roi.get_id());
  EXPECT_EQ(retrievedRoi->get_name(), roi.get_name());
  EXPECT_EQ(retrievedRoi->get_polygon(), roi.get_polygon());
  EXPECT_EQ(retrievedRoi->get_is_inside(), roi.get_is_inside());
  EXPECT_EQ(retrievedRoi->get_opacity(), roi.get_opacity());

  // get it again, this time it should access regionOfInterestCache
  retrievedRoi = db::DBManager::getRegionOfInterest(roi.get_id());

  EXPECT_EQ(retrievedRoi->get_id(), roi.get_id());
  EXPECT_EQ(retrievedRoi->get_name(), roi.get_name());
  EXPECT_EQ(retrievedRoi->get_polygon(), roi.get_polygon());
  EXPECT_EQ(retrievedRoi->get_is_inside(), roi.get_is_inside());
  EXPECT_EQ(retrievedRoi->get_opacity(), roi.get_opacity());
}

TEST_F(DBManagerTest, Zero_InsertAndGetROI) {

  iv_annotation::RegionOfInterest roi = roiZero();
  ASSERT_TRUE(db::DBManager::insertROI(roi));

  auto retrievedRoi = db::DBManager::getRegionOfInterest(roi.get_id());

  EXPECT_EQ(retrievedRoi->get_id(), roi.get_id());
  EXPECT_EQ(retrievedRoi->get_name(), roi.get_name());
  EXPECT_EQ(retrievedRoi->get_polygon(), roi.get_polygon());
  EXPECT_EQ(retrievedRoi->get_is_inside(), roi.get_is_inside());
  EXPECT_EQ(retrievedRoi->get_opacity(), roi.get_opacity());
}

TEST_F(DBManagerTest, Max_InsertAndGetROI) {

  iv_annotation::RegionOfInterest roi = roiMax();
  ASSERT_TRUE(db::DBManager::insertROI(roi));

  auto retrievedRoi = db::DBManager::getRegionOfInterest(roi.get_id());

  EXPECT_EQ(retrievedRoi->get_id(), roi.get_id());
  EXPECT_EQ(retrievedRoi->get_name(), roi.get_name());
  EXPECT_EQ(retrievedRoi->get_polygon(), roi.get_polygon());
  EXPECT_EQ(retrievedRoi->get_is_inside(), roi.get_is_inside());
  EXPECT_EQ(retrievedRoi->get_opacity(), roi.get_opacity());
}

// BoundingBox

TEST_F(DBManagerTest, Random_InsertAndGetBoundingBoxes) {
  int numEntries = 1000;
  QVector<std::shared_ptr<iv_annotation::BoundingBox>> bboxes;
  QVector<std::shared_ptr<iv_annotation::BoundingBox>> bboxesCopy;
  for (int i = 0; i < numEntries; ++i) {
    auto bb = std::make_shared<iv_annotation::BoundingBox>(*bboxRandom());
    bboxes.push_back(bb);
    bboxesCopy.push_back(std::make_shared<iv_annotation::BoundingBox>(*bb));
  }

  QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> rois;
  uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
  auto roiPtr = std::make_shared<iv_annotation::RegionOfInterest>(roiRandom());
  rois.append(roiPtr);

  ASSERT_TRUE(dbManager->insertBoundingBoxes(bboxes, rois, 30, timestamp));

  auto [annotation, roi, fps] = dbManager->getBoundingBoxes(timestamp, timestamp + 1);
  ASSERT_EQ(annotation.size(), 1);

  auto bbInFrame = annotation[0];
  ASSERT_EQ(bbInFrame.size(), bboxesCopy.size());

  for (size_t i = 0; i < bboxesCopy.size(); ++i) {
    ASSERT_EQ(*bbInFrame.at(i), *bboxesCopy.at(i));
  }
}

TEST_F(DBManagerTest, Zero_InsertAndGetBoundingBoxes) {
  // TODO adjust bboxZero etc.
  QVector<std::shared_ptr<iv_annotation::BoundingBox>> bboxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxZero())};
  QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> rois;
  uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();

  ASSERT_TRUE(dbManager->insertBoundingBoxes(bboxes, rois, 30, timestamp));

  auto [retrievedAnnotations, retrievedRois, fps] = dbManager->getBoundingBoxes(timestamp, timestamp + 1);
  ASSERT_EQ(retrievedAnnotations.size(), 1);

  auto retrievedBboxes = retrievedAnnotations[0];
  ASSERT_EQ(retrievedBboxes.size(), bboxes.size());

  for (size_t i = 0; i < bboxes.size(); ++i) {
    ASSERT_EQ(*retrievedBboxes.at(i), *bboxes.at(i));
  }
}

TEST_F(DBManagerTest, Null_InsertAndGetBoundingBoxes) {
  QVector<std::shared_ptr<iv_annotation::BoundingBox>> bboxes;
  QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> rois;
  uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();

  ASSERT_TRUE(dbManager->insertBoundingBoxes(bboxes, rois, 30, timestamp));

  auto [retrievedAnnotations, retrievedRois, fps] = dbManager->getBoundingBoxes(timestamp, timestamp + 1);
  ASSERT_EQ(retrievedAnnotations.size(), 1);

  auto retrievedBboxes = retrievedAnnotations[0];
  ASSERT_TRUE(retrievedBboxes.empty());
}

TEST_F(DBManagerTest, Max_InsertAndGetBoundingBoxes) {
  QVector<std::shared_ptr<iv_annotation::BoundingBox>> bboxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxMax())};
  QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> rois;
  uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();

  ASSERT_TRUE(dbManager->insertBoundingBoxes(bboxes, rois, 30, timestamp));

  auto [retrievedAnnotations, retrievedRois, fps] = dbManager->getBoundingBoxes(timestamp, timestamp + 1);
  ASSERT_EQ(retrievedAnnotations.size(), 1);

  auto retrievedBboxes = retrievedAnnotations[0];
  ASSERT_EQ(retrievedBboxes.size(), bboxes.size());

  for (size_t i = 0; i < bboxes.size(); ++i) {
    ASSERT_EQ(*retrievedBboxes.at(i), *bboxes.at(i));
  }
}

TEST_F(DBManagerTest, GetTimeRangeTuple) {
  uint64_t oldestTimestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  uint64_t middleTimestamp = oldestTimestamp + 5000000000;
  uint64_t newestTimestamp = oldestTimestamp + 10000000000;

  QVector<std::shared_ptr<iv_annotation::BoundingBox>> oldestBBoxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxRandom())};
  ASSERT_TRUE(dbManager->insertBoundingBoxes(oldestBBoxes, {}, 30, oldestTimestamp));

  QVector<std::shared_ptr<iv_annotation::BoundingBox>> middleBBoxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxRandom())};
  ASSERT_TRUE(dbManager->insertBoundingBoxes(middleBBoxes, {}, 30, middleTimestamp));

  QVector<std::shared_ptr<iv_annotation::BoundingBox>> newestBBoxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxRandom())};
  ASSERT_TRUE(dbManager->insertBoundingBoxes(newestBBoxes, {}, 30, newestTimestamp));

  auto [retrievedOldestTimestamp, retrievedNewestTimestamp, retrievedFps] = dbManager->getTimeRangeTuple();
  EXPECT_EQ(retrievedOldestTimestamp, oldestTimestamp);
  EXPECT_EQ(retrievedNewestTimestamp, newestTimestamp);
  EXPECT_EQ(retrievedFps, 30);
}


TEST_F(DBManagerTest, GetTimeRange) {
  auto oldestTime = QDateTime::currentDateTimeUtc();
  auto newestTime = QDateTime::currentDateTimeUtc().addSecs(20);

  uint64_t oldestTimestamp = static_cast<uint64_t>(oldestTime.toMSecsSinceEpoch()) * 1000000; // Convert milliseconds to nanoseconds
  uint64_t newestTimestamp = static_cast<uint64_t>(newestTime.toMSecsSinceEpoch()) * 1000000;

  QVector<std::shared_ptr<iv_annotation::BoundingBox>> oldestBBoxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxRandom())};
  ASSERT_TRUE(dbManager->insertBoundingBoxes(oldestBBoxes, {}, 30, oldestTimestamp));

  QVector<std::shared_ptr<iv_annotation::BoundingBox>> newestBBoxes = {std::make_shared<iv_annotation::BoundingBox>(*bboxRandom())};
  ASSERT_TRUE(dbManager->insertBoundingBoxes(newestBBoxes, {}, 30, newestTimestamp));

  TimeRange timeRange = dbManager->getTimeRange();

  EXPECT_EQ(timeRange.getStartTime(), oldestTimestamp);
  EXPECT_EQ(timeRange.getEndTime(), newestTimestamp);
}
