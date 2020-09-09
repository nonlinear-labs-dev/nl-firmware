#pragma once
#include "AsyncCommandLine.h"
#include <vector>
#include <memory>

class SpawnAsyncCommandLine
{
 public:
  static bool spawn(const std::vector<std::string>& command, std::function<void(const std::string&)> success,
                    std::function<void(const std::string&)> error);

 private:
  friend class AsyncCommandLine;
  static std::list<std::unique_ptr<AsyncCommandLine>> m_commands;
};