#pragma once

#include <proxies/hwui/controls/Label.h>

class PresetInfoCaption : public Label
{
 private:
  typedef Label super;

 public:
  PresetInfoCaption(const Rect &pos);
  ~PresetInfoCaption() override;

  bool redraw(FrameBuffer &fb) override;

 private:
  virtual void setFontColor(FrameBuffer &fb) const override;
  virtual shared_ptr<Font> getFont() const override;
  virtual int getFontHeight() const override;

  void onBankSelection();
  void onBankChanged();

  sigc::connection m_bankConnection;
};
