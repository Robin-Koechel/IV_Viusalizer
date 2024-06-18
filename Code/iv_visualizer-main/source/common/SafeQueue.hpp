// Inspired by Adam Kecskes https://github.com/K-Adam/SafeQueue
// MIT License
// and modified to use QT

#pragma once

#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

template <class T>
class SafeQueue {

  QQueue<T> q;

  mutable QMutex mtx;
  QWaitCondition cv;

  QWaitCondition sync_wait;
  bool finish_processing = false;
  int sync_counter = 0;

  void DecreaseSyncCounter() {
    if (--sync_counter == 0) {
      sync_wait.wakeOne();
    }
  }

 public:
  typedef typename QQueue<T>::size_type size_type;

  SafeQueue() {}

  ~SafeQueue() { Finish(); }

  void Produce(T&& item) {

    QMutexLocker lock(&mtx);

    q.enqueue(std::move(item));
    cv.wakeOne();
  }

  size_type Size() const {

    QMutexLocker lock(&mtx);

    return q.size();
  }

  [[nodiscard]] bool Consume(T& item) {

    QMutexLocker lock(&mtx);

    if (q.isEmpty()) {
      return false;
    }

    item = std::move(q.dequeue());

    return true;
  }

  [[nodiscard]] bool ConsumeSync(T& item) {

    QMutexLocker lock(&mtx);

    sync_counter++;

    cv.wait(&mtx, [&] { return !q.isEmpty() || finish_processing; });

    if (q.isEmpty()) {
      DecreaseSyncCounter();
      return false;
    }

    item = std::move(q.dequeue());

    DecreaseSyncCounter();
    return true;
  }

  void Finish() {
    QMutexLocker locker(&mtx);

    finish_processing = true;
    cv.wakeAll();

    while (sync_counter != 0) {
      sync_wait.wait(&mtx);
    }

    finish_processing = false;
  }

  bool IsEmpty() const {
    QMutexLocker lock(&mtx);
    return q.isEmpty();
}
};
