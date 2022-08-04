#include "DirectLoadUseCases.h"
#include "EditBufferUseCases.h"
#include <proxies/hwui/HWUI.h>
#include <presets/PresetManager.h>
#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/Preset.h>

DirectLoadUseCases::DirectLoadUseCases(DirectLoadSetting *setting)
    : m_setting { setting }
{
}

void doLoadWithoutLoadToPartIfEnabled(DirectLoadSetting *s)
{
  if(s->get())
  {
    if(auto pm = Application::get().getPresetManager())
    {
      auto eb = pm->getEditBuffer();

      if(auto selectedPreset = pm->getSelectedPreset())
      {
        if(eb->getUUIDOfLastLoadedPreset() != selectedPreset->getUuid())
        {
          EditBufferUseCases useCase(*pm->getEditBuffer());
          useCase.load(selectedPreset);
        }
      }
    }
  }
}

void DirectLoadUseCases::toggleDirectLoadFromHWUI(HWUI *pHwui)
{
  auto pm = Application::get().getPresetManager();
  auto vgManager = Application::get().getVGManager();

  if(vgManager->isInLoadToPart())
  {
    auto vg = vgManager->getCurrentVoiceGroup();
    auto load = vgManager->getPresetPartSelection(vg);

    m_setting->toggle();

    if(m_setting->get())
    {
      if(auto preset = load->m_preset)
      {
        EditBufferUseCases useCase(*pm->getEditBuffer());
        useCase.loadToPart(preset, load->m_voiceGroup, vg);
      }
    }
  }
  else
  {
    m_setting->toggle();
    doLoadWithoutLoadToPartIfEnabled(m_setting);
  }
}

void DirectLoadUseCases::toggleDirectLoadFromBaseUnit()
{
  m_setting->toggle();
  doLoadWithoutLoadToPartIfEnabled(m_setting);
}

void DirectLoadUseCases::disableDirectLoad()
{
  m_setting->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
}

void DirectLoadUseCases::enableDirectLoadFromWebUI(Preset *pPreset, VoiceGroup from, VoiceGroup to)
{
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  EditBufferUseCases useCase(*eb);

  m_setting->set(BooleanSettings::BOOLEAN_SETTING_TRUE);

  if(pPreset)
  {
    useCase.loadToPart(pPreset, from, to);
  }
  else if(auto selectedPreset = pm->getSelectedPreset())
  {
    if(eb->getUUIDOfLastLoadedPreset() != selectedPreset->getUuid())
    {
      useCase.load(selectedPreset);
    }
  }
}

void DirectLoadUseCases::setDirectLoad(bool b)
{
  m_setting->set(b ? BooleanSettings::BOOLEAN_SETTING_TRUE : BooleanSettings::BOOLEAN_SETTING_FALSE);
  doLoadWithoutLoadToPartIfEnabled(m_setting);
}

void DirectLoadUseCases::enableDirectLoadWithoutPreset()
{
  m_setting->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
}
