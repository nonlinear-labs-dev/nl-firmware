#include "DualParameterScreenCarousel.h"
#include "NeverHighlitButton.h"
#include "MiniParameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/voice-group-master-group/VGMasterParameter.h>

DualParameterScreenCarousel::DualParameterScreenCarousel(const Rect &r)
    : ParameterCarousel(r)
{
  m_editbufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &DualParameterScreenCarousel::rebuild));
}

void DualParameterScreenCarousel::setup(Parameter *selectedParameter)
{
  clear();

  auto type = Application::get().getPresetManager()->getEditBuffer()->getType();
  if(type == SoundType::Split)
    setupMasterParameters({18700, 11247, 11248});
  else
    setupMasterParameters({11247, 11248});

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

void DualParameterScreenCarousel::rebuild()
{
  auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  auto s = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);
  setup(s);
}

void DualParameterScreenCarousel::setupMasterParameters(const std::vector<Parameter::ID> &parameters)
{
  const auto ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  int yPos = ySpaceing;

  auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();

  const auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);

  for(auto p : parameters)
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(p, vg);
    auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));
    miniParam->setSelected(param == selected);
    addControl(miniParam);
    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}
