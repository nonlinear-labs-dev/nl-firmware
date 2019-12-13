#include "MonoParameterCarousel.h"
#include "NeverHighlitButton.h"
#include "MiniParameter.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>

MonoParameterCarousel::MonoParameterCarousel(const Rect &rect)
    : ParameterCarousel(rect)
{
  m_editbufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &MonoParameterCarousel::rebuild));
}

void MonoParameterCarousel::setup(Parameter *selectedParameter)
{
  clear();

  if(auto monoParam = dynamic_cast<MonoParameter<Parameter> *>(selectedParameter))
  {
    setupMonoControls(monoParam);
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

VoiceGroup MonoParameterCarousel::getVoiceGroup()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(eb->getType() == SoundType::Split)
    return Application::get().getHWUI()->getCurrentVoiceGroup();
  else
    return VoiceGroup::I;
}

void MonoParameterCarousel::rebuild()
{
  auto s = Application::get().getPresetManager()->getEditBuffer()->getSelected();
  setup(s);
}

void MonoParameterCarousel::setupMonoControls(MonoParameter<Parameter> *parameter)
{
  const auto ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  int yPos = ySpaceing;

  const auto vg = getVoiceGroup();

  for(int i : { 364, 365, 366, 367 })
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID({ i, vg });
    auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));
    miniParam->setSelected(param == parameter);
    addControl(miniParam);
    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}

MonoParameterCarousel::~MonoParameterCarousel()
{
  m_editbufferConnection.disconnect();
}
