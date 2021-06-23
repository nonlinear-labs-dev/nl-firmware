#include "DirectLoadUseCases.h"
#include <proxies/hwui/HWUI.h>

DirectLoadUseCases::DirectLoadUseCases(DirectLoadSetting *setting)
    : m_setting { setting }
{
}

void DirectLoadUseCases::toggleDirectLoadFromHWUI(HWUI *pHwui)
{
  if(pHwui->isInLoadToPart())
  {
    auto vg = pHwui->getCurrentVoiceGroup();
    auto load = pHwui->getPresetPartSelection(vg);
    m_setting->toggleWithinLoadToPart(load);
  }
  else
  {
    m_setting->toggleWithoutLoadToPart();
  }
}

void DirectLoadUseCases::toggleDirectLoadFromBaseUnit()
{
  m_setting->toggleWithoutLoadToPart();
}

void DirectLoadUseCases::disableDirectLoad()
{
  m_setting->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
}

void DirectLoadUseCases::enableDirectLoadFromWebUI(Preset *pPreset, VoiceGroup from, VoiceGroup to)
{
  if(pPreset)
    m_setting->enableWithExplicitLoadToPart(pPreset, from, to);
  else
    m_setting->enableWithoutLoadToPart();
}
