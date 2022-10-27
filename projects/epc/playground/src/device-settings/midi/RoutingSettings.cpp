#include "RoutingSettings.h"
#include "ParameterId.h"
#include "parameter_declarations.h"
#include <tools/StringTools.h>
#include <playcontroller/playcontroller-defs.h>

RoutingSettings::RoutingSettings(Settings& s)
    : Setting(s)
    , m_data {}
{
  for(auto& row : m_data)
  {
    row.fill(true);
  }
}

bool RoutingSettings::getState(RoutingSettings::tRoutingIndex hwIdx, RoutingSettings::tAspectIndex settingIdx) const
{
  return m_data[static_cast<size_t>(hwIdx)][static_cast<size_t>(settingIdx)];
}

void RoutingSettings::load(const Glib::ustring& text, Initiator initiator)
{
  if(!text.empty())
  {
    auto entries = StringTools::splitStringOnAnyDelimiter(text, '\n');
    auto idx = 0;
    for(const auto& entry : entries)
    {
      auto map = StringTools::splitStringOnAnyDelimiter(entry, ',');
      for(auto settingIdx = 0; settingIdx < static_cast<size_t>(tAspectIndex::LENGTH); settingIdx++)
      {
        try
        {
          m_data.at(idx).at(settingIdx) = map.at(settingIdx) == "1";
        }
        catch(...)
        {
          nltools::Log::error(__PRETTY_FUNCTION__,
                              "Encountered exception while trying to update Routing for indices: (entry)(aspect)", idx,
                              settingIdx);
        }
      }

      idx++;
    }
  }

  if(initiator == Initiator::EXPLICIT_LOAD)
  {
    sanitizeReceiveHWSourcesAndPC();
  }
}

Glib::ustring RoutingSettings::save() const
{
  Glib::ustring ret;
  for(auto routing : m_data)
  {
    for(auto entry : routing)
    {
      ret += entry ? "1," : "0,";
    }
    ret += "\n";
  }
  return ret;
}

Glib::ustring RoutingSettings::getDisplayString() const
{
  return { "Not Used" };
}

void RoutingSettings::setState(RoutingSettings::tRoutingIndex hwIdx, RoutingSettings::tAspectIndex settingIdx,
                               bool state)
{
  auto updatePair = [](bool& toSet, bool& other, bool value)
  {
    if(value)
      other = false;
    toSet = value;
  };

  auto idx = static_cast<size_t>(hwIdx);
  auto& val = m_data.at(idx).at(static_cast<size_t>(settingIdx));
  auto changed = false;

  if(val != state)
  {
    if(hwIdx != tRoutingIndex::Notes && settingIdx == tAspectIndex::RECEIVE_SPLIT)
      updatePair(val, m_data.at(idx).at(static_cast<size_t>(tAspectIndex::RECEIVE_PRIMARY)), state);
    else if(hwIdx != tRoutingIndex::Notes && settingIdx == tAspectIndex::RECEIVE_PRIMARY)
      updatePair(val, m_data.at(idx).at(static_cast<size_t>(tAspectIndex::RECEIVE_SPLIT)), state);
    else
      val = state;

    changed = true;
  }

  auto wasSanitized = sanitizeReceiveHWSourcesAndPC();

  if(changed || wasSanitized)
  {
    notify();
  }
}

const RoutingSettings::tData& RoutingSettings::getRaw() const
{
  return m_data;
}

void RoutingSettings::setAllValues(bool value)
{
  auto old_values = m_data;

  for(auto& entry : m_data)
    for(auto& aspect : entry)
      aspect = value;

  sanitizeReceiveHWSourcesAndPC();
  if(m_data != old_values)
    notify();
}

bool RoutingSettings::getState(const ParameterId& hwId, RoutingSettings::tAspectIndex aspect) const
{
  tRoutingIndex idx = tRoutingIndex::LENGTH;
  switch(hwId.getNumber())
  {
    case C15::PID::Pedal_1:
      idx = tRoutingIndex::Pedal1;
      break;
    case C15::PID::Pedal_2:
      idx = tRoutingIndex::Pedal2;
      break;
    case C15::PID::Pedal_3:
      idx = tRoutingIndex::Pedal3;
      break;
    case C15::PID::Pedal_4:
      idx = tRoutingIndex::Pedal4;
      break;
    case C15::PID::Bender:
      idx = tRoutingIndex::Bender;
      break;
    case C15::PID::Aftertouch:
      idx = tRoutingIndex::Aftertouch;
      break;
    case C15::PID::Ribbon_1:
      idx = tRoutingIndex::Ribbon1;
      break;
    case C15::PID::Ribbon_2:
      idx = tRoutingIndex::Ribbon2;
      break;
    case C15::PID::Ribbon_3:
      idx = tRoutingIndex::Ribbon3;
      break;
    case C15::PID::Ribbon_4:
      idx = tRoutingIndex ::Ribbon4;
      break;
    default:
      break;
  }
  return getState(idx, aspect);
}

void RoutingSettings::setAllAspectsForIndex(RoutingSettings::tRoutingIndex hwIdx, bool state)
{
  bool anyChanged = false;
  auto& entry = m_data[static_cast<int>(hwIdx)];
  for(auto& aspect : entry)
  {
    anyChanged |= (aspect != state);
    aspect = state;
  }

  auto changed = sanitizeReceiveHWSourcesAndPC();

  if(anyChanged || changed)
    notify();
}

bool RoutingSettings::sanitizeReceiveHWSourcesAndPC()
{
  using HW = RoutingSettings::tRoutingIndex;

  auto changed = false;

  for(auto idx : { HW::Aftertouch, HW::Bender, HW::Ribbon1, HW::Ribbon2, HW::Ribbon3, HW::Ribbon4, HW::Pedal1,
                   HW::Pedal2, HW::Pedal3, HW::Pedal4, HW::ProgramChange })
  {
    auto i = static_cast<size_t>(idx);
    auto rP = static_cast<size_t>(tAspectIndex::RECEIVE_PRIMARY);
    auto rS = static_cast<size_t>(tAspectIndex::RECEIVE_SPLIT);
    auto receivePrim = m_data[i][rP];
    auto receiveSplit = m_data[i][rS];
    if(receivePrim && receiveSplit)
    {
      m_data[i][rS] = false;
      changed = true;
    }
  }

  return changed;
}
