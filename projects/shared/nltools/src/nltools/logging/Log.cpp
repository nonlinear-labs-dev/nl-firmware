#include <nltools/logging/Log.h>
#include <iostream>
#include <mutex>
#include <malloc.h>

nltools::Log::Level nltools::Log::s_level = nltools::Log::Level::Debug;
bool nltools::Log::s_flush = false;

void nltools::Log::memUsage(nltools::Log::Level l, const char *location)
{
  if(s_level <= l)
  {
    auto bytes = mallinfo().uordblks;
    memUsage(l, location, bytes);
  }
}

void nltools::Log::memUsage(Level l, const char *location, size_t bytes)
{
  if(s_level <= l)
  {
    auto humanReadable = bytes;
    int idx = 0;

    while(humanReadable > 1024)
    {
      humanReadable /= 1024;
      idx++;
    }

    static std::string unit[] = { "B", "kB", "MB", "GB", "TB" };

    if(idx)
      output<LogMode::InsertSpacesAndAppendNewLine>("MemoryUsage: ", location, bytes, "B, ", humanReadable, unit[idx]);
    else
      output<LogMode::InsertSpacesAndAppendNewLine>("MemoryUsage: ", location, bytes, "B");
  }
}

void nltools::Log::flush()
{
  std::cout << std::flush;
}

void nltools::Log::print(const std::string &str)
{
  static std::mutex m;
  std::unique_lock<std::mutex> l(m);
  std::cout << str;
}

void nltools::Log::printNewLine()
{
  static std::mutex m;
  std::unique_lock<std::mutex> l(m);
  std::cout << std::endl;
}

static uint32_t s_indent = 0;

nltools::Log::MemUsageLogger::MemUsageLogger(Level l, const char *loc)
    : level(l)
    , loc(loc)
    , begin(mallinfo().uordblks)
{
  s_indent++;
}

nltools::Log::MemUsageLogger::~MemUsageLogger()
{
  s_indent--;
  auto u = getUsage();
  if(u > 0)
    memUsage(level, (std::string(s_indent, ' ') + loc).c_str(), u);
}

int64_t nltools::Log::MemUsageLogger::getUsage() const
{
  return mallinfo().uordblks - begin;
}
