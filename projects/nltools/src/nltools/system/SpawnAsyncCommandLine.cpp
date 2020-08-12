#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/logging/Log.h>

SpawnAsyncCommandLine::SpawnAsyncCommandLine(const std::string& command)
    : m_command { command }
{
  nltools::Log::warning(__LINE__, __PRETTY_FUNCTION__, command);
  m_thread = std::thread([&]() {
    nltools::Log::warning("Spawning Async Command", command);
    SpawnCommandLine cmd(command);
    nltools::Log::warning("Async Command", command, "out:", cmd.getStdOutput(), "err:", cmd.getStdError());
  });
}

SpawnAsyncCommandLine::~SpawnAsyncCommandLine()
{
  nltools::Log::warning("Detatching Command Line: ", m_command);
  m_thread.detach();
}
