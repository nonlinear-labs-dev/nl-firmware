#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/MonoParameterOverlay.h>
#include "MonoParameterItem.h"

MonoParameterItem::MonoParameterItem(const std::string &caption, const Parameter *parameter, const Rect &rect)
    : EditorItem(caption, rect)
    , m_parameter{ parameter }
{

}

MenuOverlay *MonoParameterItem::createOverlay()
{
  auto pos = getPosition();
  pos.setHeight(pos.getHeight() - 2);
  pos.setTop(pos.getTop() + 1);
  return new MonoParameterOverlay(pos, m_parameter);
}

const Parameter *MonoParameterItem::getParameter() const
{
  return m_parameter;
}
