#include "nltools/system/SpawnAsyncCommandLine.h"
#include <algorithm>
#include <nltools/logging/Log.h>

std::list<std::unique_ptr<AsyncCommandLine>> SpawnAsyncCommandLine::m_commands;

size_t SpawnAsyncCommandLine::spawn(Glib::RefPtr<Glib::MainContext> ctx, const std::vector<std::string>& command,
                                    const std::function<void(const std::string&)>& success,
                                    const std::function<void(const std::string&)>& error)
{
  m_commands.emplace_back(std::make_unique<AsyncCommandLine>(ctx, command, success, error));
  return removeDone();
}

size_t SpawnAsyncCommandLine::getNumCommands()
{
  return m_commands.size();
}

void SpawnAsyncCommandLine::clear()
{
  m_commands.clear();
}

size_t SpawnAsyncCommandLine::removeDone()
{
  m_commands.remove_if([](auto& c) { return !c->isRunning(); });
  return m_commands.size();
}
