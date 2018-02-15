#pragma once

#include <sigc++/connection.h>
#include <functional>
#include <thread>
#include "FileTools.h"

class BackgroundJob
{
public:
    BackgroundJob(sigc::signal<void()> cb) : m_sigNews(cb) {
    };

    ~BackgroundJob()
    {
      m_close = true;
      m_thread.join();
    }

    virtual void start() = 0;

    void stop()
    {
      m_close = false;
    }

    sigc::connection onGotNews(std::function<void()> cb) // ‘cb’ will be called from bg thread whenever there are news;
    {
      cb();
      return 
    };

protected:
    std::thread m_thread;
    sigc::signal<void()> m_sigNews;
    bool m_close;
};


class FileCrawlerJob : public BackgroundJob
{
public:
    FileCrawlerJob(std::string dir, std::function<bool(std::experimental::filesystem::directory_entry)> filter, sigc::signal<void()> cb) :
            BackgroundJob(cb), directory(dir), fileFilter(filter), m_thread(start)
    {
    }

    void start() override
    {
      const std::string directory(directory);
      auto it = std::experimental::filesystem::recursive_directory_iterator(std::experimental::filesystem::path(directory.c_str()));
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
            onGotNews(m_sigNews);
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

    FileTools::FileList copyList()
    {
      return list;
    }

protected:
    FileTools::FileList list;
    std::string directory;
    std::function<bool(std::experimental::filesystem::directory_entry)> fileFilter;
};