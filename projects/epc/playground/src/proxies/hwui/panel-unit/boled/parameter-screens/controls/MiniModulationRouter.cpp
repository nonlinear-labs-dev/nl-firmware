#include "MiniModulationRouter.h"
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <proxies/hwui/controls/LabelRegular8.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MiniParameterBarSlider.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>

MiniModulationRouter::MiniModulationRouter(ModulationRoutingParameter *param, const Rect &rect)
    : super(rect)
    , m_param(param)
{

  if(auto targetParam = param->getTargetParameter())
  {
    addControl(new LabelRegular8(targetParam->getMiniParameterEditorName(), Rect(0, 1, rect.getWidth(), 11)));
    addControl(new MiniParameterBarSlider(param, Rect(0, 13, rect.getWidth(), 2)));

    param->getSourceParameter()->onParameterChanged(mem_fun(this, &MiniModulationRouter::onSourceParameterChanged));
  }
}

void MiniModulationRouter::onSourceParameterChanged(const Parameter *p)
{
  if(auto a = dynamic_cast<const PhysicalControlParameter *>(p))
  {
    setHighlight(a->getUiSelectedModulationRouter() == m_param->getID());
  }
}
