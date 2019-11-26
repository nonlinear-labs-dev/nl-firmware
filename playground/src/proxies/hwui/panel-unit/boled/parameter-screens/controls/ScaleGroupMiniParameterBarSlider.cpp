#include <groups/ScaleGroup.h>
#include <parameters/Parameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ScaleGroupMiniParameterBarSlider.h>

ScaleGroupMiniParameterBarSlider::ScaleGroupMiniParameterBarSlider(Parameter *param, const Rect &rect)
    : super(param, rect)
{
  m_connection
      = param->getParentGroup()->onGroupChanged(sigc::mem_fun(this, &ScaleGroupMiniParameterBarSlider::setDirty));
}

ScaleGroupMiniParameterBarSlider::~ScaleGroupMiniParameterBarSlider()
{
  m_connection.disconnect();
}

tDisplayValue ScaleGroupMiniParameterBarSlider::getDrawValue() const
{
  if(auto param = getParameter())
    if(auto scale = dynamic_cast<const ScaleGroup *>(param->getParentGroup()))
      return scale->isAnyOffsetChanged() ? 1.0 : 0.0;

  return 0.0;
}
