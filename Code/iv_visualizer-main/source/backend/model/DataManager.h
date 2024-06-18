#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <memory>
#include <QMap>
#include <QString>
#include <QMutex>
#include <QList>
#include "TimeRange.h"
#include "Image.h"
#include "RegionOfInterest.h"
#include "DataFrame.h"
#include "SafeQueue.hpp"
#include "DBManager.h"
#include "FSManager.h"
#include "DBManager.h"
#include "FSManager.h"

/**
 * @brief The DataManager class manages data frames and regions of interest.
 */
class DataManager {
 private:
  static bool enableBuffer; /**< Flag indicating whether the buffer is enabled. */
  static bool enableAsyncQueue; /**< Flag indicating whether the queue is filled asynchronously or not. */
  static int minBufferSize; /**< Minimum buffer size. */
  static int maxBufferSize; /**< Maximum buffer size. */
  static int batchSize; /**< Batch size. */
  static QMap<QString, std::shared_ptr<DataManager>> instances; /**< Map of DataManager instances. */
  void cleanupBuffer(); /**< Cleans up the buffer. */
  friend class DataManagerTest; /**< Friend class for testing private members. */

  QString url; /**< URL for DataManager instance. */
  db::DBManager database; /**< Database manager instance. */
  fsm::FSManager filesystem; /**< Filesystem manager instance. */
  QList<std::shared_ptr<DataFrame>> buffer; /**< List of data frames in buffer. */
  QMutex bufferMutex; /**< Mutex for buffer access synchronization. */
  bool cleanupInProgress; /**< Flag indicating cleanup in progress. */


 public:
  explicit DataManager(const QString &url_); /**< Constructor with URL parameter. */ // TODO: should be private
  ~DataManager(); /**< Destructor. */


  /**
   * @brief getInstance returns an instance of DataManager associated with the specified URL.
   * @param url URL for the DataManager instance.
   * @return DataManager instance.
   */
  static std::shared_ptr<DataManager> getInstance(const QString& url);


  /**
   * @brief addDataFrame adds a data frame to the DataManager buffer.
   * @param data Pointer to the DataFrame to be added.
   */
  void addDataFrame(std::shared_ptr<DataFrame> data);


  /**
   * @brief addRegionOfInterest adds a region of interest to the DataManager.
   * @param roi Region of interest to be added.
   */
  void addRegionOfInterest(const iv_annotation::RegionOfInterest &roi);


  /**
   * @brief getBufferSize returns the current size of the buffer.
   * @return Size of the buffer.
   */
  int getBufferSize();


  /**
   * @brief async fillOutputQueue fills the output queue with data frames within a specified range.
   * @param start Starting index of the range.
   * @param end Ending index of the range.
   * @param queue Pointer to the output queue to be filled.
   */
  void fillOutputQueue(uint64_t start, uint64_t end, std::shared_ptr<SafeQueue<std::shared_ptr<DataFrame>>> queue);


  /**
   * @brief Blocking: getOutputQueue gets the output queue containing data frames within a specified range.
   * @param start Starting index of the range.
   * @param end Ending index of the range.
   * @return Pointer to the output queue.
   */
  std::shared_ptr<SafeQueue<std::shared_ptr<DataFrame>>> getOutputQueue(uint64_t start, uint64_t end);


  /**
   * @brief gets first and last timestamp of the annotation in the database.
   * @return Tuple containing the start and end times and the frame rate.
   */
  std::tuple<cass_int64_t, cass_int64_t, float> getTimeRangeTuple();


  /**
   * @brief getTimeRange returns the time range between the first and last timestamp of the annotation in the database
   * @return TimeRange object containing the start and end times and the frame rate.
   */
  TimeRange getTimeRangeAnnotations();

  /**
   * @brief getTimeRangeVideos returns the time range between the first and last timestamp of available video data
   * @return TimeRange object containing the start and end times and the frame rate.
   */
  TimeRange getTimeRangeVideos();
};

#endif  // DATAMANAGER_H
