#include "VoiceGroupMasterParameterCarousel.h"
#include "NeverHighlitButton.h"
#include "MiniParameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterModulateableParameter.h>
#include <sigc++/sigc++.h>
#include <parameter_declarations.h>

VoiceGroupMasterParameterCarousel::VoiceGroupMasterParameterCarousel(const Rect &r)
    : ParameterCarousel(r)
{
  m_editbufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &VoiceGroupMasterParameterCarousel::rebuild));

  m_selectConnection = Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::hide(sigc::mem_fun(this, &VoiceGroupMasterParameterCarousel::rebuild)));
}

VoiceGroupMasterParameterCarousel::~VoiceGroupMasterParameterCarousel()
{
  m_selectConnection.disconnect();
  m_editbufferConnection.disconnect();
}

void VoiceGroupMasterParameterCarousel::setup(Parameter *selectedParameter)
{
  clear();

  auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();

  switch(Application::get().getPresetManager()->getEditBuffer()->getType())
  {
    default:
    case SoundType::Single:
      setupMasterParameters({ { C15::PID::Voice_Grp_Volume, vg }, { C15::PID::Voice_Grp_Tune, vg } });
      break;
    case SoundType::Split:
      setupMasterParameters({ { C15::PID::Voice_Grp_Volume, vg },
                              { C15::PID::Voice_Grp_Tune, vg },
                              { C15::PID::Split_Split_Point, vg } });
      break;
    case SoundType::Layer:
      setupMasterParameters({ { C15::PID::Voice_Grp_Volume, vg },
                              { C15::PID::Voice_Grp_Tune, vg },
                              { C15::PID::Voice_Grp_Fade_From, vg },
                              { C15::PID::Voice_Grp_Fade_Range, vg } });
      break;
  }

  if(getNumChildren() == 0)
    addControl(new NeverHighlitButton("", Buttons::BUTTON_D));
  else
    setHighlight(true);

  setDirty();
}

void VoiceGroupMasterParameterCarousel::rebuild()
{
  auto s = Application::get().getPresetManager()->getEditBuffer()->getSelected(getHWUI()->getCurrentVoiceGroup());
  setup(s);
}

void VoiceGroupMasterParameterCarousel::setupMasterParameters(const std::vector<ParameterId> &parameters)
{
  m_currentCarouselContentIDs = {};
  for(auto& i: parameters)
    m_currentCarouselContentIDs.emplace_back(i.getNumber());

  const int ySpacing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  const auto numMissing = 4 - parameters.size();
  auto yPos = ySpacing + (numMissing * (miniParamHeight + ySpacing));

  const auto selected
      = Application::get().getPresetManager()->getEditBuffer()->getSelected(getHWUI()->getCurrentVoiceGroup());

  for(const auto &p : parameters)
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(p);
    if(param)
    {
      auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));
      miniParam->setSelected(param == selected);
      addControl(miniParam);
      yPos += ySpacing;
      yPos += miniParamHeight;
    }
  }
}
