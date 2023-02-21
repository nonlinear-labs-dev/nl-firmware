#pragma once
#include <proxies/hwui/controls/ControlWithChildren.h>
#include "presets/Preset.h"

class Label;

class PresetPropertyDisplay : public ControlWithChildren
{
 public:
  PresetPropertyDisplay();
  void updateFrom(const Glib::ustring& text);
  void setPosition(const Rect& rect) override;

 private:
  Label* m_type;
  Label* m_unison;
  Label* m_mono;
  Label* m_scale;
};
