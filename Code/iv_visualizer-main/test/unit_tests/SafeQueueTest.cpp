// TODO: modify to work without mask
// #include <gtest/gtest.h>

// #include <thread>
// #include <vector>

// #include "DataFrame.h"
// #include "SafeQueue.hpp"

// class SafeQueueTest : public ::testing::Test {
//  protected:
//   SafeQueue<DataFrame> queue;
// };

// DataFrame createDataFrameInstance() {
//   QString url = "http://example.com";
//   QImage qImage(1920, 1080, QImage::Format_ARGB32);
//   uint64_t timestamp = QDateTime::currentDateTime();
//   Image image(qImage, timestamp);

//   std::vector<Mask> masks;
//   masks.push_back(Mask(true, QPolygon(), "Mask1", 1));
//   masks.push_back(Mask(false, QPolygon(), "Mask2", 2));

//   DataFrame dataFrame(url, image, masks);
//   return dataFrame;
// }


// TEST_F(SafeQueueTest, StressTestConcurrentAccess) {
//   const int numberOfThreads = 100;
//   std::vector<std::thread> threads;
//   std::mutex mtx;
//   std::condition_variable cv;
//   bool ready = false;
//   std::atomic<int> producedCount(0);
//   std::atomic<int> consumedCount(0);

//   auto threadWork = [this, &mtx, &cv, &ready, &producedCount, &consumedCount]() {
//     std::unique_lock<std::mutex> lock(mtx);
//     cv.wait(lock, [&ready] { return ready; });
//     DataFrame item = createDataFrameInstance();
//     if (rand() % 2 == 0) {
//       queue.Produce(std::move(item));
//       producedCount++;
//     } else {
//       if (queue.Consume(item)) {
//         consumedCount++;
//       }
//     }
//   };

//   for (int i = 0; i < numberOfThreads; ++i) {
//     threads.emplace_back(threadWork);
//   }

//   {
//     std::lock_guard<std::mutex> lock(mtx);
//     ready = true;
//   }
//   cv.notify_all();

//   for (auto& th : threads) {
//     th.join();
//   }

//   EXPECT_GE(producedCount.load(), consumedCount.load());
// }
