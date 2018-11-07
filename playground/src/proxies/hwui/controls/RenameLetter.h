#pragma once

#include "LabelRegular8.h"

class TextEditUsageMode;

class RenameLetter : public LabelRegular8
{
 private:
  typedef LabelRegular8 super;

 public:
  RenameLetter(shared_ptr<TextEditUsageMode> mode, int buttonID, const Rect &pos);

  StringAndSuffix getText() const override;
  bool redraw(FrameBuffer &fb) override;

 private:
  int getXOffset() const override;
  bool highlightLetter() const;

  shared_ptr<TextEditUsageMode> m_mode;
  int m_buttonID;
};
