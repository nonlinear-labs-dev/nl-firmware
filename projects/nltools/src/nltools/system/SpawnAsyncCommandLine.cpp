#include "nltools/system/SpawnAsyncCommandLine.h"
#include <algorithm>

std::list<std::unique_ptr<AsyncCommandLine>> SpawnAsyncCommandLine::m_commands;

bool SpawnAsyncCommandLine::spawn(const std::vector<std::string>& command,
                                  std::function<void(const std::string&)> success,
                                  std::function<void(const std::string&)> error)
{
  m_commands.emplace_back(std::make_unique<AsyncCommandLine>(command, success, error));
  m_commands.remove_if([](auto& c) { return !c->isRunning(); });
  return true;
}