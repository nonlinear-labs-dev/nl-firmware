#include "DebugLevel.h"
#include "Settings.h"
#include "http/UpdateDocumentMaster.h"
#include "Application.h"
#include "proxies/lpc/LPCProxy.h"
#include <iostream>
#include <sys/time.h>

DebugLevel::DebugLevel(Settings &settings)
    : super(settings, DebugLevels::DEBUG_LEVEL_WARNING)
{
}

DebugLevel::~DebugLevel() = default;

bool DebugLevel::set(DebugLevels m)
{
  switch(m)
  {
    case DebugLevels::DEBUG_LEVEL_SILENT:
      nltools::Log::setLevel(nltools::Log::Silent);
      break;

    case DebugLevels::DEBUG_LEVEL_ERROR:
      nltools::Log::setLevel(nltools::Log::Error);
      break;

    case DebugLevels::DEBUG_LEVEL_WARNING:
      nltools::Log::setLevel(nltools::Log::Warning);
      break;

    case DebugLevels::DEBUG_LEVEL_INFO:
      nltools::Log::setLevel(nltools::Log::Info);
      break;

    case DebugLevels::DEBUG_LEVEL_DEBUG:
    case DebugLevels::DEBUG_LEVEL_GASSY:
      nltools::Log::setLevel(nltools::Log::Debug);
      break;
  }

  return super::set(m);
}

const std::vector<Glib::ustring> &DebugLevel::enumToString() const
{
  static std::vector<Glib::ustring> s_modeNames = { "silent", "error", "warning", "info", "debug", "gassy" };
  return s_modeNames;
}

const std::vector<Glib::ustring> &DebugLevel::enumToDisplayString() const
{
  static std::vector<Glib::ustring> s_modeNames = { "Silent", "Error", "Warning", "Info", "Debug", "Gassy" };
  return s_modeNames;
}

void DebugLevel::printError(const std::string &str)
{
  std::cerr << getTimestamp() << ": " << str << std::endl;
}

uint64_t DebugLevel::getTimestamp()
{
  static uint64_t epoch = getCurrentMilliseconds();
  return getCurrentMilliseconds() - epoch;
}

uint64_t DebugLevel::getCurrentMilliseconds()
{
  struct timeval spec;
  gettimeofday(&spec, nullptr);
  return spec.tv_sec * 1000 + spec.tv_usec / 1000;
}
