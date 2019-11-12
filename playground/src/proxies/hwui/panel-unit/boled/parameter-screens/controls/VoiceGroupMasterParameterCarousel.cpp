#include "VoiceGroupMasterParameterCarousel.h"
#include "NeverHighlitButton.h"
#include "MiniParameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameter.h>

VoiceGroupMasterParameterCarousel::VoiceGroupMasterParameterCarousel(const Rect &r)
    : ParameterCarousel(r)
{
  m_editbufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &VoiceGroupMasterParameterCarousel::rebuild));

  m_selectConnection = Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged(
      sigc::mem_fun(this, &VoiceGroupMasterParameterCarousel::rebuild));
}

VoiceGroupMasterParameterCarousel::~VoiceGroupMasterParameterCarousel()
{
  m_selectConnection.disconnect();
  m_editbufferConnection.disconnect();
}

void VoiceGroupMasterParameterCarousel::setup(Parameter *selectedParameter)
{
  clear();

  if(Application::get().getPresetManager()->getEditBuffer()->getType() == SoundType::Split)
    setupMasterParameters({ 10001, 10002, 10003 });
  else
    setupMasterParameters({ 10002, 10003 });

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

void VoiceGroupMasterParameterCarousel::rebuild()
{
  auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  auto s = Application::get().getPresetManager()->getEditBuffer()->getSelected();
  setup(s);
}

void VoiceGroupMasterParameterCarousel::setupMasterParameters(const std::vector<Parameter::ID> &parameters)
{
  const auto ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  const auto numMissing = 4 - parameters.size();
  int yPos = ySpaceing + (numMissing * (miniParamHeight + ySpaceing));

  const auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  const auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);

  for(auto p : parameters)
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(p, vg);
    if(!param)
      param = Application::get().getPresetManager()->getEditBuffer()->findGlobalParameterByID(p);

    auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));
    miniParam->setSelected(param == selected);
    addControl(miniParam);
    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}