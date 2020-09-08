#include "nltools/system/SpawnAsyncCommandLine.h"

std::vector<std::unique_ptr<AsyncCommandLine>> SpawnAsyncCommandLine::m_commands;

bool SpawnAsyncCommandLine::spawn(const std::vector<std::string>& command,
                                  std::function<void(const std::string&)> success,
                                  std::function<void(const std::string&)> error)
{
  m_commands.emplace_back(std::make_unique<AsyncCommandLine>(command, success, error));
  return true;
}