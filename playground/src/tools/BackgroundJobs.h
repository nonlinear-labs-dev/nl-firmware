#pragma once

#include <functional>
#include <thread>
#include "FileTools.h"

template <typename T>
class BackgroundJob
{
public:
    BackgroundJob(std::function<void (T t)> cb) : callback(cb), m_thread(nullptr) {
    };

    ~BackgroundJob()
    {
      stop();
      if(m_thread->joinable())
        m_thread->join();
    }

    virtual void start() = 0;

    void stop()
    {
      m_close = false;
    }

    bool isRunning() {
      return !m_close;
    }

protected:
    std::unique_ptr<std::thread> m_thread;
    std::function<void (T t)> callback;
    bool m_close;
};


class FileCrawlerJob : public BackgroundJob<FileTools::FileList>
{
public:
    FileCrawlerJob(std::string dir, std::function<bool(std::experimental::filesystem::directory_entry)> filter, std::function<void(FileTools::FileList fl)> callback) :
            BackgroundJob(callback), directory(dir), fileFilter(filter)
    {
      m_thread = std::make_unique<std::thread>([=]() {
        start();
      });
    }

    void start() override
    {
      const std::string dir(directory);
      auto it = std::experimental::filesystem::recursive_directory_iterator(std::experimental::filesystem::path(dir.c_str()));
      while(!m_close && it != std::experimental::filesystem::recursive_directory_iterator())
      {
        try
        {
          Glib::ustring name(it->path().string());
          DebugLevel::warning(__FILE__, name);
          if (std::experimental::filesystem::is_directory(it->path()) && name.find("/.") != Glib::ustring::npos)
          {
            DebugLevel::warning(__FILE__, name);
            it.disable_recursion_pending();
          } else if (!std::experimental::filesystem::is_directory(it->path()) && !fileFilter(*it))
          {
            list.emplace_back(*it);
            callback(list);
          }
        } catch(Glib::Error err)
        {
          DebugLevel::error(err.what(), err.code());
        } catch (...)
        {
          DebugLevel::error("non Glib::Error while traversing Thumbdrive!");
        }
        ++it;
      }
    }

protected:
    FileTools::FileList list;
    std::string directory;
    std::function<bool(std::experimental::filesystem::directory_entry)> fileFilter;
};