#include <device-settings/PedalType.h>
#include <proxies/lpc/LPCProxy.h>
#include <http/UpdateDocumentMaster.h>
#include <EHC-pedal-presets.h>
#include "Application.h"
#include "proxies/lpc/LPCProxy.h"

PedalType::PedalType(UpdateDocumentContributor &settings, uint16_t lpcKey)
    : super(settings, PedalTypes::PotTipActive)
    , m_lpcKey(lpcKey)
{
}

Glib::ustring PedalType::getDisplayString() const
{
  auto v = getAllValues<PedalTypes>();
  auto it = std::find(v.begin(), v.end(), get());
  if(it != v.end())
  {
    auto idx = std::distance(v.begin(), it);
    return getDisplayStrings().at(idx);
  }

  return "";
}

const std::vector<Glib::ustring> &PedalType::getDisplayStrings() const
{
  static std::vector<Glib::ustring> s_modeNames;

  if(s_modeNames.empty())
  {
    forEachValue<PedalTypes>([&](PedalTypes e) {
      auto index = static_cast<EHC_PRESET_ID>(e);
      if(auto p = EHC_GetPresetById(index))
        s_modeNames.emplace_back(p->displayName);
    });
  }

  return s_modeNames;
}

PedalType::~PedalType() = default;

void PedalType::sendToLPC(SendReason reason) const
{
  auto reset = reason == SendReason::SettingChanged;
  Application::get().getLPCProxy()->sendPedalSetting(m_lpcKey, get(), reset);
}

void PedalType::load(const Glib::ustring &text)
{
  static std::map<std::string, std::string> m { { "pot-tip-active", "PotTipActive" },
                                                { "pot-ring-active", "PotRingActive" },
                                                { "switch-closing", "SwitchClosing" },
                                                { "switch-opening", "SwitchOpening" } };

  auto it = m.find(text);
  if(it != m.end())
    super::load(it->second);
  else
    super::load(text);
}

bool PedalType::set(PedalTypes m)
{
  return forceset(m);
}
