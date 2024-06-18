#include "DataManager.h"
#include <QtConcurrent/QtConcurrentRun>

#include <cstdlib>

bool DataManager::enableBuffer = false;
bool DataManager::enableAsyncQueue = false;
int DataManager::minBufferSize = 20;
int DataManager::maxBufferSize = 120;
int DataManager::batchSize = 10;
QMap<QString, std::shared_ptr<DataManager>> DataManager::instances;



DataManager::DataManager(const QString& url_)
    : url(url_), cleanupInProgress(false), database(url_), filesystem(url_) {

  char* enableBufferEnv = std::getenv("DATA_MANAGER_ENABLE_BUFFER");
  char* enableAsyncQueueEnv = std::getenv("DATA_MANAGER_ENABLE_ASYNC_QUEUE");
  char* minBufferSizeEnv = std::getenv("DATA_MANAGER_MIN_BUFFER_SIZE");
  char* maxBufferSizeEnv = std::getenv("DATA_MANAGER_MAX_BUFFER_SIZE");
  char* batchSizeEnv = std::getenv("DATA_MANAGER_BATCH_SIZE");

  if (enableBufferEnv) {
    enableBuffer = std::string(enableBufferEnv) == "true";
  }
  if (enableAsyncQueueEnv) {
    enableAsyncQueue = std::string(enableAsyncQueueEnv) == "true";
  }
  if (minBufferSizeEnv) {
    minBufferSize = QString(minBufferSizeEnv).toInt();
  }
  if (maxBufferSizeEnv) {
    maxBufferSize = QString(maxBufferSizeEnv).toInt();
  }
  if (batchSizeEnv) {
    batchSize = QString(batchSizeEnv).toInt();
  }
}


DataManager::~DataManager() {
  // remove from instances TODO: Deconstructor
}


std::shared_ptr<DataManager> DataManager::getInstance(const QString& url) {
  if (!instances.contains(url)) {
    instances[url] = std::make_shared<DataManager>(url);
  }
  return instances[url];
}


void DataManager::addDataFrame(std::shared_ptr<DataFrame> data) {

  if (enableBuffer) {
    QMutexLocker locker(&bufferMutex);
    buffer.append(data);

    if (buffer.size() > maxBufferSize) {
      QtConcurrent::run([this]() {
        QMutexLocker locker(&bufferMutex);
        cleanupBuffer();
      });
    }
  } else {
    // TODO: code repetition
    uint64_t timestamp = data->getImage()->get_timestamp();
    database.insertBoundingBoxes(data->getBoundingBoxen(), data->getRegionsOfInterest(), data->getFps(), timestamp);
    filesystem.addImage(data->getImage(), data->getFps());
  }
}


void DataManager::addRegionOfInterest(const iv_annotation::RegionOfInterest& roi) {
  database.insertROI(roi);
}

// call this only if you have a mutex lock on the buffer
void DataManager::cleanupBuffer() {
  while (buffer.size() >= minBufferSize + batchSize) {
    QList<std::shared_ptr<DataFrame>> batch = buffer.mid(0, batchSize);

    for (auto data : batch) {
      uint64_t timestamp = data->getImage()->get_timestamp();
      database.insertBoundingBoxes(data->getBoundingBoxen(), data->getRegionsOfInterest(), data->getFps(), timestamp);
      filesystem.addImage(data->getImage(), data->getFps());
    }

    buffer.erase(buffer.begin(), buffer.begin() + batchSize);
  }
}


int DataManager::getBufferSize() { return buffer.size(); }


std::shared_ptr<SafeQueue<std::shared_ptr<DataFrame>>> DataManager::getOutputQueue(uint64_t start, uint64_t end) {

  auto queue = std::make_shared<SafeQueue<std::shared_ptr<DataFrame>>>();

  if (enableAsyncQueue) {
    QtConcurrent::run(
        [this, start, end, &queue]() { fillOutputQueue(start, end, queue); });
  } else {
    fillOutputQueue(start, end, queue);
  }

  return queue;
}


void DataManager::fillOutputQueue(uint64_t start, uint64_t end, std::shared_ptr<SafeQueue<std::shared_ptr<DataFrame>>> queue) {
  QVector<std::shared_ptr<Image>> images = filesystem.getImages(start, end);
  auto [boundingBoxesLists, roiLists, fps] = database.getBoundingBoxes(start, end);
  if(boundingBoxesLists.size() == 0){
    for (int i = 0; i < images.size(); ++i) {
      queue->Produce(std::make_shared<DataFrame>(url, images[i], QVector<std::shared_ptr<iv_annotation::RegionOfInterest>>(), QVector<std::shared_ptr<iv_annotation::BoundingBox>>(), 30));
    }
    return;
  }
  uint64_t size = std::min({images.size(), roiLists.size(), boundingBoxesLists.size()});
  for (int i = 0; i < size; ++i) {
      queue->Produce(std::make_shared<DataFrame>(url, images[i], roiLists[i], boundingBoxesLists[i], fps));
  }
}


std::tuple<cass_int64_t, cass_int64_t, float> DataManager::getTimeRangeTuple() {
  return database.getTimeRangeTuple();
}


TimeRange DataManager::getTimeRangeAnnotations() {
  return database.getTimeRange();
}


TimeRange DataManager::getTimeRangeVideos(){
  return filesystem.getTimeRangeOfVideos();
}
