#pragma once
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>

inline RoutingSettings* getSetting()
{
  if(Application::exists())
  {
    return Application::get().getSettings()->getSetting<RoutingSettings>();
  }
  return nullptr;
}

inline std::initializer_list<RoutingSettings::tAspectIndex> getAspectsForIndex(RoutingSettings::tRoutingIndex id)
{
  typedef RoutingSettings::tAspectIndex ASP;
  static auto aspects = { ASP::SEND_PRIMARY, ASP::RECEIVE_PRIMARY, ASP::SEND_SPLIT, ASP::RECEIVE_SPLIT, ASP::LOCAL };
  static auto pgAspects = { ASP::SEND_PRIMARY, ASP::RECEIVE_PRIMARY, ASP::SEND_SPLIT, ASP::RECEIVE_SPLIT };
  if(id == decltype(id)::ProgramChange)
    return pgAspects;
  else
    return aspects;
}