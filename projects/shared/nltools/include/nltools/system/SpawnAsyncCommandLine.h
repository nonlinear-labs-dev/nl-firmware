#pragma once
#include "AsyncCommandLine.h"
#include <vector>
#include <memory>

class SpawnAsyncCommandLine
{
 public:
  static size_t spawn(Glib::RefPtr<Glib::MainContext> ctx, const std::vector<std::string>& command,
                      const std::function<void(const std::string&)>& success,
                      const std::function<void(const std::string&)>& error);

  static size_t removeDone();
  static size_t getNumCommands();
  static void clear();

 private:
  friend class AsyncCommandLine;
  static std::list<std::unique_ptr<AsyncCommandLine>> m_commands;
};