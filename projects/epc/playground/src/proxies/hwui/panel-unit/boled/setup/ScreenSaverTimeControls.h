#pragma once
#include "SetupLabel.h"
#include "SetupEditor.h"

class ScreenSaverTimeView : public SetupLabel
{
 public:
  ScreenSaverTimeView();
  void onSettingChanged(const Setting *s);
};

class ScreenSaverTimeEditor : public SetupLabel, public SetupEditor
{
 private:
  typedef SetupLabel super;

 public:
  ScreenSaverTimeEditor();
  ~ScreenSaverTimeEditor() override;

  void setBackgroundColor(FrameBuffer &fb) const override;
  Font::Justification getJustification() const override;
  void setFontColor(FrameBuffer &fb) const override;
  bool redraw(FrameBuffer &fb) override;

 private:
  void onSettingChanged(const Setting *s);
  bool onRotary(int inc, ButtonModifiers modifiers) override;

  SetupLabel *m_label = nullptr;
};