#include <device-settings/PedalType.h>
#include <proxies/lpc/LPCProxy.h>
#include <http/UpdateDocumentMaster.h>
#include "Application.h"
#include "proxies/lpc/LPCProxy.h"

PedalType::PedalType(UpdateDocumentContributor &settings, uint16_t lpcKey)
    : super(settings, PedalTypes::PotTipActive)
    , m_lpcKey(lpcKey)
{
}

PedalType::~PedalType()
{
}

void PedalType::sendToLPC(SendReason reason) const
{
  auto reset = reason == SendReason::SettingChanged;
  Application::get().getLPCProxy()->sendPedalSetting(m_lpcKey, get(), reset);
}

const std::vector<Glib::ustring> &PedalType::enumToString() const
{
  static std::vector<Glib::ustring> s_modeNames

      = { "pot-tip-active", "pot-ring-active", "switch-closing", "switch-opening" };
  return s_modeNames;
}

const std::vector<Glib::ustring> &PedalType::enumToDisplayString() const
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
