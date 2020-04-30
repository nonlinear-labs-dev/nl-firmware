#pragma once

#include "SelectedParameterDotSlider.h"

class SelectedParameterKnubbelSlider : public SelectedParameterDotSlider
{
 private:
  typedef SelectedParameterDotSlider super;

 public:
  SelectedParameterKnubbelSlider(const Rect &rect);
void setParameter(Parameter* param) override;

 protected:
  virtual void drawHandle(FrameBuffer &fb) override;
};
