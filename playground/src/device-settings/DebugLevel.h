#pragma once

#include "EnumSetting.h"
#include <sys/time.h>
#include <nltools/logging/Log.h>

enum class DebugLevels
{
  DEBUG_LEVEL_DEBUG,
  DEBUG_LEVEL_SILENT,
  DEBUG_LEVEL_ERROR,
  DEBUG_LEVEL_WARNING,
  DEBUG_LEVEL_INFO,
  DEBUG_LEVEL_GASSY
};

inline std::ostream& operator<<(std::ostream& out, const DebugLevels& level)
{
  switch(level)
  {
    case DebugLevels::DEBUG_LEVEL_SILENT:
      out << "silent";
      break;

    case DebugLevels::DEBUG_LEVEL_ERROR:
      out << "!!! ERROR !!!";
      break;

    case DebugLevels::DEBUG_LEVEL_WARNING:
      out << "WARNING";
      break;

    case DebugLevels::DEBUG_LEVEL_DEBUG:
      out << "Debug";
      break;

    case DebugLevels::DEBUG_LEVEL_INFO:
      out << "info";
      break;

    case DebugLevels::DEBUG_LEVEL_GASSY:
      out << "gassy";
      break;
  }

  return out;
}

class DebugLevel : public EnumSetting<DebugLevels>
{
 private:
  typedef EnumSetting<DebugLevels> super;

 public:
  DebugLevel(Settings& settings);
  virtual ~DebugLevel();

  bool set(DebugLevels m) override;

  static DebugLevels getLevel();

  template <typename... tArgs> static void gassy(const tArgs&... args)
  {
    nltools::Log::notify(args...);
  }

  template <typename... tArgs> static void info(const tArgs&... args)
  {
    nltools::Log::info(args...);
  }

  template <typename... tArgs> static void debug(const tArgs&... args)
  {
    nltools::Log::debug(args...);
  }

  template <typename... tArgs> static void warning(const tArgs&... args)
  {
    nltools::Log::warning(args...);
  }

  template <typename... tArgs> static void error(const tArgs&... args)
  {
    nltools::Log::error(args...);
  }

 private:
  DebugLevel(const DebugLevel& other);
  DebugLevel& operator=(const DebugLevel&);

  const std::vector<Glib::ustring>& enumToString() const override;
  const std::vector<Glib::ustring>& enumToDisplayString() const override;
};
