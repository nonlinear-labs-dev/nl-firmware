#pragma once

#include "proxies/hwui/controls/Label.h"

class Application;
class PresetBank;

class BankNameLabel : public Label
{
 private:
  typedef Label super;

 public:
  BankNameLabel(const Rect &pos);
  virtual ~BankNameLabel();

  void updateLabel(shared_ptr<PresetBank> newBank);

 protected:
  void setBackgroundColor(FrameBuffer &fb) const override;
  void setFontColor(FrameBuffer &fb) const override;
  Font::Justification getJustification() const override;
  int getXOffset() const override;
  virtual StringAndSuffix shortenStringIfNeccessary(shared_ptr<Font> font, const StringAndSuffix &text) const override;
};
