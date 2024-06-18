#ifndef FSMANAGER_H
#define FSMANAGER_H

#include <QImage>
#include <QRegularExpression>
#include <QString>
#include <filesystem>
#include <chrono>
#include <thread>
#include <iostream>
#include <map> // TODO: migrate to QMap
#include <QMap>
#include <memory>
#include <string>
#include <vector>
#include <QPainter>
#include "TimeRange.h"

#include "Image.h"
#include "Encoder.h"

namespace fsm {
namespace fs = std::filesystem;

/**
 * @brief The FSManager class manages file system operations related to images.
 */
class FSManager {
 public:
  /**
   * @brief Constructs an FSManager object with the specified URL.
   * @param url The URL to manage.
   */
  explicit FSManager(const QString& url);

  /**
   * @brief Destroys the FSManager object.
   */
  ~FSManager();

  /**
   * @brief Adds an image to the manager with the given frame rate.
   * @param image Pointer to the Image object to add.
   * @param a_fps The frame rate of the image.
   */
  void addImage(std::shared_ptr<Image> image, int a_fps);

  /**
   * @brief Retrieves images within the specified range.
   * @param start The starting index of the range.
   * @param end The ending index of the range.
   * @return A QVector containing pointers to the retrieved images.
   */
  QVector<std::shared_ptr<Image>> getImages(uint64_t start, uint64_t end);

  /**
   * @brief gets the TimeRange of available video data
   * @return the Timerange of available data
  */
  TimeRange getTimeRangeOfVideos();

 private:
  QString m_url; /**< The URL managed by the FSManager. */
  std::string path = "./output/"; /**< The default output path. */
  Encoder* activeWriter; /**< Pointer to the active Encoder. */
  uint64_t activeWriterTimestamp = 0; /**< Timestamp of the active writer. */
  int currentFrames = 0; /**< Current number of frames. */
  int maxFrames = 1500; /**< Maximum number of frames. */
  uint64_t ttl = 1209600000000; /**< Time to live for images. */
  int cleanupInterval = 600; /**< Interval for cleanup routine. */
  bool stopCleanup = false; /**< Flag to stop cleanup routine. */
  std::thread cleanupThread; /**< Thread for cleanup routine. */
  std::map<uint64_t, std::shared_ptr<Image>> buffer; /**< Buffer for images. */
  std::map<uint64_t, Encoder> readerMap; /**< Map of reader objects. */

  /**
   * @brief correct the folder name.
   * @param input The input folder name.
   * @return The corrected folder name.
   */
  static QString validateFolderName(const QString& input);

  /**
   * @brief Cleanup routine for managing resources.
   * @param stop Reference to a boolean flag to stop the cleanup routine.
   */
  void cleanupRoutine(bool &stop);
};
}  // namespace fsm

#endif  // FSMANAGER_H
