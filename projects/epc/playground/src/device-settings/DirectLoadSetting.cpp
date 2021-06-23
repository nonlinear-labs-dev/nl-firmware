#include "DirectLoadSetting.h"
#include <device-settings/Settings.h>
#include <Application.h>
#include <presets/PresetManager.h>

DirectLoadSetting::DirectLoadSetting(Settings &settings)
    : BooleanSetting(settings, false)
{
}

void DirectLoadSetting::load(const Glib::ustring &text, Initiator initiator)
{
  if(initiator == Initiator::EXPLICIT_LOAD)
  {
    m_isLoading = true;
    BooleanSetting::load(text, initiator);
    m_isLoading = false;
  }
  else
  {
    BooleanSetting::load(text, initiator);
  }
}

bool DirectLoadSetting::set(BooleanSettings m)
{
  auto ret = EnumSetting::set(m);

  if(!m_isLoading)
  {
    if(m == BooleanSettings::BOOLEAN_SETTING_TRUE)
    {
      if(m_currentOnDirectLoadWasEnabledCB)
      {
        m_currentOnDirectLoadWasEnabledCB();
      }
    }
  }

  return ret;
}

void DirectLoadSetting::toggleWithinLoadToPart(PresetPartSelection *part)
{
  m_currentOnDirectLoadWasEnabledCB = [part]()
  {
    if(auto pm = Application::get().getPresetManager())
    {
      if(auto preset = part->m_preset)
      {
        EditBufferUseCases useCase(pm->getEditBuffer());
        useCase.undoableLoadToPart(preset, part->m_voiceGroup, part->m_focusedVoiceGroup);
      }
    }
  };

  toggle();
  m_currentOnDirectLoadWasEnabledCB = nullptr;
}

void DirectLoadSetting::toggleWithoutLoadToPart()
{
  m_currentOnDirectLoadWasEnabledCB = []()
  {
    if(auto pm = Application::get().getPresetManager())
    {
      if(auto selectedPreset = pm->getSelectedPreset())
      {
        EditBufferUseCases useCase(pm->getEditBuffer());
        useCase.undoableLoad(selectedPreset);
      }
    }
  };

  toggle();
  m_currentOnDirectLoadWasEnabledCB = nullptr;
}

void DirectLoadSetting::enableWithExplicitLoadToPart(Preset *pPreset, VoiceGroup from, VoiceGroup to)
{
  m_currentOnDirectLoadWasEnabledCB = [pPreset, from, to]
  {
    EditBufferUseCases useCase(Application::get().getPresetManager()->getEditBuffer());
    useCase.undoableLoadToPart(pPreset, from, to);
  };

  set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  m_currentOnDirectLoadWasEnabledCB = nullptr;
}

void DirectLoadSetting::enableWithoutLoadToPart()
{
  m_currentOnDirectLoadWasEnabledCB = []()
  {
      if(auto pm = Application::get().getPresetManager())
      {
        if(auto selectedPreset = pm->getSelectedPreset())
        {
          EditBufferUseCases useCase(pm->getEditBuffer());
          useCase.undoableLoad(selectedPreset);
        }
      }
  };

  set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  m_currentOnDirectLoadWasEnabledCB = nullptr;
}
