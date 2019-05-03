#include "DebugLevel.h"
#include "Settings.h"
#include "http/UpdateDocumentMaster.h"
#include "Application.h"
#include "proxies/lpc/LPCProxy.h"

DebugLevels lastSetLevel = DebugLevels::DEBUG_LEVEL_WARNING;

DebugLevel::DebugLevel(Settings &settings)
    : super(settings, DebugLevels::DEBUG_LEVEL_WARNING)
{
  DebugLevel::gassy(__PRETTY_FUNCTION__, G_STRLOC);
}

DebugLevel::~DebugLevel()
{
}

bool DebugLevel::set(DebugLevels m)
{
  if(super::set(m))
  {
    lastSetLevel = m;
    DebugLevel::gassy(__PRETTY_FUNCTION__, G_STRLOC, save());
    return true;
  }
  return false;
}

DebugLevels DebugLevel::getLevel()
{
  return lastSetLevel;
}

const std::vector<Glib::ustring> &DebugLevel::enumToString() const
{
  static std::vector<Glib::ustring> s_modeNames = { "debug", "silent", "error", "warning", "info", "gassy" };
  return s_modeNames;
}

const std::vector<Glib::ustring> &DebugLevel::enumToDisplayString() const
{
  static std::vector<Glib::ustring> s_modeNames = { "Debug", "Silent", "Error", "Warning", "Info", "Gassy" };
  return s_modeNames;
}
