#include "FSManager.h"

namespace fsm {
namespace fs = std::filesystem;


FSManager::FSManager(const QString& url) {

  // get environment variables
  char* maxFrameCStr = std::getenv("VIDEO_LENGTH_IN_FRAMES");
  if (maxFrameCStr != nullptr) {
    maxFrames = std::stoi(maxFrameCStr);
  }

  char* outputPathCStr = std::getenv("VIDEO_PATH");
  if (outputPathCStr != nullptr) {
    path = std::string(outputPathCStr);
  }

  char* ttlCStr = std::getenv("TTL_VIDEO");
  if (ttlCStr != nullptr) {
    ttl = std::stoi(ttlCStr);
  }

  char* cleanupIntervalCStr = std::getenv("TTL_CLEANUP_INTERVAL");
  if (cleanupIntervalCStr != nullptr) {
    cleanupInterval = std::stoi(cleanupIntervalCStr);
  }

  // set other variables
  m_url = validateFolderName(url);
  path += m_url.toStdString();
  currentFrames = 0;
  fs::path dir = path;

  if (fs::exists(path) && fs::is_directory(path)) {
    // create Encoder from existing files
    fs::path lastEntryPath;
     for (const auto& entry : fs::directory_iterator(path)) {
      const std::string filename = entry.path().filename().string();
      if (!entry.is_regular_file()) {
        continue;
      }
      uint64_t timestamp = std::stoull(filename);
      Encoder reader(timestamp, QString::fromStdString(entry.path()), 0);  
      readerMap.emplace(timestamp, std::move(reader));
      lastEntryPath = entry.path();
    }
    if (!lastEntryPath.empty()) {
        if (std::remove(lastEntryPath.string().c_str()) != 0) {
            perror("Error deleting last entry");
        } else {
            auto lastIterator = readerMap.end();
            --lastIterator;
            readerMap.erase(lastIterator);
        }
    }
  } else {
    fs::create_directories(path);
  }

  //cleanupThread = std::thread(&FSManager::cleanupRoutine, this, std::ref(stopCleanup));

  this->activeWriter = nullptr;
}


FSManager::~FSManager() {
  stopCleanup = true;
  //cleanupThread.join();

  if (activeWriter != nullptr) {
    delete activeWriter;
  }
}


QString FSManager::validateFolderName(const QString& input) {
  QString output;
  int maxLength = qMin(input.length(), 254);
  for (int i = 0; i < maxLength; ++i) {
    const QChar& ch = input.at(i);
    if (ch == '/' || ch == '\\' || ch == '\0' || ch.category() == QChar::Other_Control) {
      output += "[" + QString::number(ch.unicode()) + "]";
    } else {
      output += ch;
    }
  }
  return output;
}


void FSManager::addImage(std::shared_ptr<Image> image, int a_fps) {
  // create new file from time to time or when settings changed
  if (this->currentFrames >= this->maxFrames || this->activeWriter == nullptr ||
      image->height() != activeWriter->get_height() ||
      image->width() != activeWriter->get_width() || activeWriter->get_fps() != a_fps) {

    // put last file in read mode
    if (this->activeWriter != nullptr) {
      this->activeWriter->finish_writing_mp4();
      Encoder reader(activeWriter->get_timestamp(), activeWriter->get_path(), 0);
      readerMap.emplace(activeWriter->get_timestamp(), std::move(reader));
      delete this->activeWriter;
    }

    // create new file
    std::string filename = std::to_string(image->get_timestamp());
    this->activeWriterTimestamp = image->get_timestamp();
    this->activeWriter = new Encoder(activeWriterTimestamp, QString::fromStdString(path + "/" + filename), a_fps);
    this->currentFrames = 0;
    for (auto it = buffer.begin(); it != buffer.end(); ) {
        it = buffer.erase(it);
    }
  }

  // add frame to file and buffer
  this->buffer.emplace(image->get_timestamp(), image);
  this->activeWriter->add_image(image);
  this->currentFrames++;
}


inline bool isBetween(uint64_t value, uint64_t lower, uint64_t upper) {
    return value >= lower && value <= upper;
}


QVector<std::shared_ptr<Image>> FSManager::getImages(uint64_t requestedStart, uint64_t requestedEnd) {
 QVector<std::shared_ptr<Image>> images;
  // for each video file
  for (auto& [fileStartTimestamp, reader] : readerMap) {
    uint64_t fileEndTimestamp = fileStartTimestamp + reader.get_frame_count() - 1;
    QVector<std::shared_ptr<Image>> newFrames;

    // determine correct video file                                                         fST       fET
    if (isBetween(fileStartTimestamp, requestedStart, requestedEnd) || //                    |-----<---|--->  first frame in video
        isBetween(fileEndTimestamp, requestedStart, requestedEnd) || //                  <---|-->------|      last frame in video
        (fileStartTimestamp <= requestedStart && fileEndTimestamp >= requestedEnd)) { // <---|---------|--->  all frames in Video
      newFrames = reader.get_frames(requestedStart, requestedEnd);
    }
    images.append(newFrames);
  }
  auto lower = buffer.lower_bound(requestedStart);
  auto upper = buffer.upper_bound(requestedEnd);
  // for each image between start and end
  for (auto it = lower; it != upper; ++it) {
    images.push_back(it->second);
  }

  if (images.isEmpty()) {
    uint64_t currentTimeStamp = requestedStart;
    while(currentTimeStamp <= requestedEnd){
      auto placeholderImage = std::make_shared<Image>(640, 480, currentTimeStamp);
      placeholderImage->fill(QColor(100, 100, 100));
      images.append(placeholderImage);
      currentTimeStamp += 1000000000/30;
    }
  }

  return images;
}


void FSManager::cleanupRoutine(bool& stop) {
  while (!stop) {

    uint64_t threshold = 0;
    auto now = std::time(nullptr);
    if (now < ttl) {
      threshold = 0;
    } else {
      threshold = now - ttl;
    }
    // remove from map
    for (auto it = readerMap.begin(); it != readerMap.end();) {
      if (it->first < threshold) {
        it = readerMap.erase(it);
      } else {
        ++it;
      }
    }

    // remove from filesystem
    for (const auto& entry : fs::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        std::string filename = entry.path().filename().string();
        try {
          std::time_t fileTimestamp = std::stoll(filename);
          if (fileTimestamp < threshold) {
            fs::remove(entry.path());
          }
        } catch (const std::exception& e) {
          std::cerr << "Error: Could not remove file: " << filename << std::endl;
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(cleanupInterval));
  }
}

TimeRange FSManager::getTimeRangeOfVideos(){
  if (!readerMap.empty()) {
    uint64_t firstTimeStamp = readerMap.begin()->first;
    auto lastIterator = --readerMap.end();
    uint64_t secondTimeStamp = lastIterator->second.get_frame_count() +lastIterator->second.get_timestamp();
    if(!buffer.empty()){
      auto lastIteratorBuffer = --buffer.end();
      secondTimeStamp = lastIteratorBuffer->first;
    }
    TimeRange range(firstTimeStamp, secondTimeStamp,readerMap.begin()->second.get_fps());
    return range;
  }
  else{
    TimeRange range(0, 0, 0);
    return range;
  }
  
  
}

}  // namespace fsm
