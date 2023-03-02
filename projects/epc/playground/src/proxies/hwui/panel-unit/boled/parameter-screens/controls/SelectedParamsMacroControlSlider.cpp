#include "SelectedParamsMacroControlSlider.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/ModulateableParameter.h"
#include "parameters/ParameterFactory.h"
#include <sigc++/sigc++.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <proxies/hwui/HWUI.h>

SelectedParamsMacroControlSlider::SelectedParamsMacroControlSlider(const Rect &rect)
    : super(rect)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedParamsMacroControlSlider::setTargetParameter)), vg);
}

SelectedParamsMacroControlSlider::~SelectedParamsMacroControlSlider()
{
}

void SelectedParamsMacroControlSlider::setTargetParameter(Parameter *param)
{
  if(m_targetParameter != param)
  {
    m_targetParameter = param;

    if(m_targetParameter)
    {
      m_targetParamValueConnection.disconnect();
      m_targetParamValueConnection = m_targetParameter->onParameterChanged(
          sigc::mem_fun(this, &SelectedParamsMacroControlSlider::onTargetParamValueChanged));
    }
    else
    {
      setParameter(nullptr);
    }
  }
}

void SelectedParamsMacroControlSlider::onTargetParamValueChanged(const Parameter *param)
{
  if(const auto *modP = dynamic_cast<const ModulateableParameter *>(param))
  {
    auto src = modP->getModulationSource();
    auto srcParamID = ParameterFactory::modSrcToParamId(src);
    setParameter(Application::get().getPresetManager()->getEditBuffer()->findParameterByID(srcParamID));
  }
  else
  {
    setParameter(nullptr);
  }
}

void SelectedParamsMacroControlSlider::setParameter(Parameter *param)
{
  Slider::setParameter(param);
  if(param)
  {
    auto eb = Application::get().getPresetManager()->getEditBuffer();
    setVisible(!param->isDisabled());
  }
}

bool SelectedParamsMacroControlSlider::isVisible() const
{
  return getParameter() && super::isVisible();
}
