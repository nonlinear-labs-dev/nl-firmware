#include <groups/ScaleGroup.h>
#include <parameters/Parameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ScaleGroupMiniParameterBarSlider.h>

ScaleGroupMiniParameterBarSlider::ScaleGroupMiniParameterBarSlider(Parameter *param, const Rect &rect)
    : super(param, rect)
{
  param->getParentGroup()->onGroupChanged(sigc::mem_fun(this, &ScaleGroupMiniParameterBarSlider::setDirty));
}

ScaleGroupMiniParameterBarSlider::~ScaleGroupMiniParameterBarSlider()
{
}

tDisplayValue ScaleGroupMiniParameterBarSlider::getDrawValue() const
{
  auto scale = static_cast<const ScaleGroup *>(getParameter()->getParentGroup());
  return scale->isAnyOffsetChanged() ? 1.0 : 0.0;
}
