#pragma once

#include <functional>
#include <thread>
#include <assert.h>
#include <mutex>
#include "FileTools.h"

class BackgroundJob
{
 protected:
  typedef std::function<void(void)> tCallback;

 public:
  BackgroundJob(tCallback cb);
  virtual ~BackgroundJob();

  virtual void start();
  virtual bool iterate() = 0;
  void killMe();
  bool isRunning();

 protected:
  std::thread m_thread;
  tCallback callback;
  bool m_close = false;
};

namespace fs = std::experimental::filesystem;

class FileCrawlerJob : public BackgroundJob
{
 protected:
  typedef std::function<bool(fs::directory_entry)> tFilterFunction;

 public:
  FileCrawlerJob(std::string dir, tFilterFunction filter, tCallback cb);

  bool iterate() override;
  FileTools::FileList copyData();

 protected:
  std::mutex m;
  FileTools::FileList list;
  std::function<bool(fs::directory_entry)> fileFilter;
  fs::recursive_directory_iterator it;
};
