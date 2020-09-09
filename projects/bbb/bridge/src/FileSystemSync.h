#pragma once

#include <future>
#include <condition_variable>
#include <mutex>

class FileSystemSync
{
 public:
  FileSystemSync();
  ~FileSystemSync();

 private:
  void doBackgroundSyncing();

  std::future<void> m_task;
  std::condition_variable m_cond;
  std::mutex m_mutex;
  bool m_cancel { false };
  bool m_dirty { false };
};
