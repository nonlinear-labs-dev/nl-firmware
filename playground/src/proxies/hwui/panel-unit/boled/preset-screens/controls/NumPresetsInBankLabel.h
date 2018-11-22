#pragma once

#include "PresetLabel.h"

class NumPresetsInBankLabel : public Label
{
 private:
  typedef Label super;

 public:
  NumPresetsInBankLabel(const Rect &pos);
  virtual ~NumPresetsInBankLabel();

 private:
  void updateText();
  void setFontColor(FrameBuffer &fb) const override;
};
