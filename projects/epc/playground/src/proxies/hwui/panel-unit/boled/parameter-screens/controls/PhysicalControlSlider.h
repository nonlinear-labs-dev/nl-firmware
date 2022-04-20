#pragma once

#include "SelectedParameterDotSlider.h"

class HardwareSourceSendParameter;
class PhysicalControlParameter;

class PhysicalControlSlider : public SelectedParameterDotSlider
{
 private:
  typedef SelectedParameterDotSlider super;

 public:
  explicit PhysicalControlSlider(const Rect& rect);
  bool redraw(FrameBuffer& fb) override;
  void setParameter(Parameter* param) override;

 protected:
  void onParameterSelectionChanged(Parameter* p);
  void drawSliderBackground(FrameBuffer &fb) override;
  bool isLocalDisabledForThisParameter();
  bool redrawLocalDisabled(FrameBuffer& buffer);

  HardwareSourceSendParameter* m_snd = nullptr;
  PhysicalControlParameter* m_hw = nullptr;

  bool m_shouldHighlightSendParameter = false;

  sigc::connection m_sndParamConnection;
  sigc::connection m_hwParamConnection;
};
