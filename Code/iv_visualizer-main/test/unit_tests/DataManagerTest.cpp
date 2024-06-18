
#include <gtest/gtest.h>

#include <QDateTime>
#include <QImage>
#include <QMutex>
#include <QRandomGenerator>
#include <QThread>
#include <QWaitCondition>
#include <condition_variable>
#include <mutex>

#include "DataManager.h"
#include "RegionOfInterest.h"
#include "gtest/gtest.h"

class DataManagerTest : public ::testing::Test {
 protected:
  virtual void TearDown() {
    DataManager::instances.clear();
  }

  iv_annotation::BoundingBox* bboxRandom() {
    cv::Size2i imageSize(1920, 1080);

    int width = QRandomGenerator::global()->bounded(1, imageSize.width);
    int height = QRandomGenerator::global()->bounded(1, imageSize.height);

    int x = QRandomGenerator::global()->bounded(0, imageSize.width - width);
    int y = QRandomGenerator::global()->bounded(0, imageSize.height - height);

    std::vector<int> coords = {x, y, width, height};
    float confidence = QRandomGenerator::global()->generateDouble();
    QString label =
        "RandomLabel_" + QString::number(QRandomGenerator::global()->generate());
    iv_annotation::BoundingBox::Format format = iv_annotation::BoundingBox::Format::TLWH;
    int rotation = QRandomGenerator::global()->bounded(0, 360);

    return new iv_annotation::BoundingBox(
        coords, confidence, label, imageSize, format, rotation);
  }

  int timeAdd = 0;
  std::shared_ptr<DataFrame> createDataFrameInstance(QString url) {
    auto qImage = std::make_shared<QImage>(800, 600, QImage::Format_RGB888);
    qImage->fill(Qt::blue);
    uint64_t timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    auto image = std::make_shared<Image>(qImage.get(), timestamp + timeAdd);
    timeAdd += 10000;

    QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest;

    QVector<std::shared_ptr<iv_annotation::BoundingBox>> boundingBoxes;
    boundingBoxes.append(std::shared_ptr<iv_annotation::BoundingBox>(bboxRandom()));

    return std::make_shared<DataFrame>(url, image, regionsOfInterest, boundingBoxes);
  }
};


TEST_F(DataManagerTest, UniqueInstancePerURL) {
  QString url1 =
      "rtsp://admin:passwort@181.112.101.124:554/Streaming/Channels/101";
  auto instance1 = DataManager::getInstance(url1);
  EXPECT_NE(nullptr, instance1);

  QString url2 =
      "rtsp://root:trowssap@42.999.10.187:554/Streaming/Channels/102";
  auto instance2 = DataManager::getInstance(url2);
  EXPECT_NE(nullptr, instance2);
  EXPECT_NE(instance1, instance2);

}


TEST_F(DataManagerTest, SameInstanceForSameURL) {
  QString url =
      "rtsp://user:passwort@181.112.101.124:554/Streaming/Channels/101";
  auto instance1 = DataManager::getInstance(url);
  auto instance2 = DataManager::getInstance(url);
  EXPECT_EQ(instance1, instance2);
}


// WARNING: This test requires at least 9GB RAM (in container)
/*
TEST_F(DataManagerTest, AddDataFrameAndOutputQueue) {
  QString url = "rtsp://addDataFrame:passwort@181.112.101.124:554/Streaming/Channels/101";
  auto dataManager = DataManager::getInstance(url);
  for (int i = 0; i < 5000; i++) {
    auto dataframe = createDataFrameInstance(url);
    dataManager->addDataFrame(dataframe);
  }

  // auto outputQueue = dataManager->getOutputQueue(0, 2000000000000000000); // blocking implemented, because thread gets canceled with google test framework
  auto queue = std::make_shared<SafeQueue<std::shared_ptr<DataFrame>>>();
  dataManager->fillOutputQueue(0, 2000000000000000000, queue);
}
*/

/* Rewrite test to comply with the latest changes
TEST_F(DataManagerTest, StressTestConcurrentAccess) {
  QString url = "rtsp://stressTestConcurrentAccess:passwort@181.112.101.124:554/";
  DataManager* instance = DataManager::getInstance(url);

  const int numberOfThreads = 100;
  QList<QThread*> threads;
  QMutex mtx;
  QWaitCondition cv;
  bool ready = false;

  auto threadWork = [&]() {
    QMutexLocker lock(&mtx);
    while (!ready) {
      cv.wait(&mtx);
    }

    if (QRandomGenerator::global()->bounded(2) == 0) {
      instance->addDataFrame(createDataFrameInstance(url));
      EXPECT_GT(instance->getBufferSize(), 0);
    } else {
      auto queue = instance->getOutputQueue(0, 10000000000);
      EXPECT_NE(queue, nullptr);
    }
  };

  for (int i = 0; i < numberOfThreads; ++i) {
    QThread* thread = QThread::create(threadWork);
    threads.append(thread);
    thread->start();
  }
  {
    QMutexLocker lock(&mtx);
    ready = true;
  }
  cv.notify_all();

  for (auto& thread : threads) {
    thread->wait();
    delete thread;
  }
}
*/