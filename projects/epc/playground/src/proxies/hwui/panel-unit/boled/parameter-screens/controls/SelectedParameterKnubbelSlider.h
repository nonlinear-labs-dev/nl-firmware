#pragma once

#include "SelectedParameterDotSlider.h"

class SelectedParameterKnubbelSlider : public SelectedParameterDotSlider
{
 private:
  typedef SelectedParameterDotSlider super;

 public:
  SelectedParameterKnubbelSlider(const Rect &rect);

 protected:
  virtual void drawHandle(FrameBuffer &fb) override;
};
