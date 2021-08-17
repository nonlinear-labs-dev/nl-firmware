#pragma once
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>

inline auto getSetting()
{
  if(Application::exists())
  {
    static auto ret = Application::get().getSettings()->getSetting<RoutingSettings>();
    return ret;
  }
  return std::shared_ptr<RoutingSettings>(nullptr);
}

inline auto getAspectsForIndex(RoutingSettings::tRoutingIndex id)
{
  typedef RoutingSettings::tAspectIndex ASP;
  static auto aspects = { ASP::SEND_PRIMARY, ASP::RECEIVE_PRIMARY, ASP::SEND_SPLIT, ASP::RECEIVE_SPLIT, ASP::LOCAL };
  static auto pgAspects = { ASP::SEND_PRIMARY, ASP::RECEIVE_PRIMARY, ASP::SEND_SPLIT, ASP::RECEIVE_SPLIT };
  if(id == decltype(id)::ProgramChange)
    return pgAspects;
  else
    return aspects;
}