#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/MonoParameterOverlay.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include "MonoParameterItem.h"

MonoParameterItem::MonoParameterItem(const std::string &caption, const Parameter *parameter, const Rect &rect)
    : EditorItem(caption, rect)
    , m_parameter{ parameter }
{
  auto rightHalf = getPosition();
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(rightHalf.getWidth());
  addControl(new ParameterValueLabel(parameter, rightHalf));
}

MenuOverlay *MonoParameterItem::createOverlay()
{
  auto pos = getPosition();
  pos.setLeft(253 - (pos.getWidth() / 2));
  pos.setWidth((pos.getWidth() / 2) - 1);
  pos.setHeight(pos.getHeight() - 2);
  pos.setTop(pos.getTop() + 1);
  return new MonoParameterOverlay(pos, m_parameter);
}

bool MonoParameterItem::redraw(FrameBuffer &fb) {
  return EditorItem::redraw(fb);
}

const Parameter *MonoParameterItem::getParameter() const
{
  return m_parameter;
}
