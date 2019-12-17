#include "MonoParameterCarousel.h"
#include "NeverHighlitButton.h"
#include "MiniParameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include <groups/MonoGroup.h>

MonoParameterCarousel::MonoParameterCarousel(const Rect &rect)
    : ParameterCarousel(rect)
{
  m_editbufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &MonoParameterCarousel::rebuild));

  m_voiceGroupConnection = Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::hide(sigc::mem_fun(this, &MonoParameterCarousel::rebuild)));
}

void MonoParameterCarousel::setup(Parameter *selectedParameter)
{
  clear();

  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(MonoGroup::isMonoParameter(selectedParameter))
  {
    setupMonoControls(selectedParameter);
  }

  if(getNumChildren() == 0)
  {
    addControl(new NeverHighlitButton("", Rect(0, 51, 58, 11)));
  }
  else
  {
    setHighlight(true);
  }

  setDirty();
}

void MonoParameterCarousel::rebuild()
{
  auto s = Application::get().getPresetManager()->getEditBuffer()->getSelected();
  setup(s);
}

MonoParameterCarousel::~MonoParameterCarousel()
{
  m_editbufferConnection.disconnect();
  m_voiceGroupConnection.disconnect();
}

void MonoParameterCarousel::setupMonoControls(Parameter *parameter)
{
  const auto ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  int yPos = ySpaceing;

  const auto soundType = Application::get().getPresetManager()->getEditBuffer()->getType();
  for(const auto &id : getParameterIdsForMode(soundType))
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(id);
    auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));
    miniParam->setSelected(param == parameter);
    addControl(miniParam);
    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}

std::vector<ParameterId> MonoParameterCarousel::getParameterIdsForMode(SoundType type)
{
  auto currentVG = Application::get().getHWUI()->getCurrentVoiceGroup();

  switch(type)
  {
    case SoundType::Split:
      return { { 364, currentVG }, { 365, currentVG }, { 366, currentVG }, { 367, currentVG } };
    case SoundType::Layer:
      return { { 364, VoiceGroup::I }, { 365, VoiceGroup::I }, { 366, VoiceGroup::I }, { 367, currentVG } };
    case SoundType::Single:
      return { { 364, VoiceGroup::I }, { 365, VoiceGroup::I }, { 366, VoiceGroup::I }, { 367, VoiceGroup::I } };
  }

  nltools_assertNotReached();
}
