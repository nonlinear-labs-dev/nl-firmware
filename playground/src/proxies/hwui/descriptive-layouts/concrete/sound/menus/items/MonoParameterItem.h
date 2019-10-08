#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/EditorItem.h>

class Parameter;

class MonoParameterItem : public EditorItem
{
public:
  MonoParameterItem(const std::string &caption, const Parameter *parameter, const Rect &rect);
  MenuOverlay *createOverlay() override;
  const Parameter* getParameter() const;

  bool redraw(FrameBuffer &fb) override;

private:
  const Parameter* m_parameter;
};
