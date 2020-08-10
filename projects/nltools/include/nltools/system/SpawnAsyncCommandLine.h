#pragma once
#include <nltools/system/SpawnCommandLine.h>
#include <thread>

class SpawnAsyncCommandLine
{
 public:
  explicit SpawnAsyncCommandLine(const std::string& command);
  ~SpawnAsyncCommandLine();

 private:
  const std::string m_command;
  std::thread m_thread;
};
