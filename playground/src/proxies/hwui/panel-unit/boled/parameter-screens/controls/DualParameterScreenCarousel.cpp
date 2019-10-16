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

  if(Application::get().getPresetManager()->getEditBuffer()->getType() == SoundType::Split)
    setupMasterParameters({ 18700, 11247, 11248 });
  else
    setupMasterParameters({ 11247, 11248 });

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
  const auto numMissing = 5 - parameters.size();
  int yPos = numMissing * miniParamHeight;

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

/*
 * #include "MonoParameterCarousel.h"
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

  if(auto monoParam = dynamic_cast<MonoParameter *>(selectedParameter))
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

bool MonoParameterCarousel::monoParametersActive()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->findParameterByID(12345, getVoiceGroup())->getDisplayString() == "On";
}

VoiceGroup MonoParameterCarousel::getVoiceGroup()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(eb->getType() == SoundType::Split)
    return Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  else
    return VoiceGroup::I;
}

void MonoParameterCarousel::rebuild()
{
  auto s = Application::get().getPresetManager()->getEditBuffer()->getSelected(getVoiceGroup());
  setup(s);
}

void MonoParameterCarousel::setupMonoControls(MonoParameter *parameter)
{
  if(!monoParametersActive())
    return;

  const auto ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  int yPos = ySpaceing;

  const auto vg = getVoiceGroup();

  for(int i : { 12345, 12346, 12347, 12348 })
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(i, vg);
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

 */