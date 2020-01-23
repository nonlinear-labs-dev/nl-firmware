#pragma once

#include "proxies/hwui/controls/Label.h"

class PresetListHeaderArrow : public Label
{
 private:
  typedef Label super;

 public:
  PresetListHeaderArrow(const Glib::ustring &text, const Rect &pos);
  ~PresetListHeaderArrow() override;

  bool redraw(FrameBuffer &fb) override;

 protected:
  void setBackgroundColor(FrameBuffer &fb) const override;
  void setFontColor(FrameBuffer &fb) const override;
  std::shared_ptr<Font> getFont() const override;
  Font::Justification getJustification() const override;
};