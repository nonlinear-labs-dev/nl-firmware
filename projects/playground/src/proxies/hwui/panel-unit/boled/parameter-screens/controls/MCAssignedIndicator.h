#pragma once
#include <proxies/hwui/controls/ControlWithChildren.h>

class Parameter;
class MacroControlParameter;
class LabelRegular8;

class MCAssignedIndicator : public ControlWithChildren
{
 public:
  MCAssignedIndicator(const Rect& r, const Parameter* p);
  bool redraw(FrameBuffer& fb) override;

 private:
  LabelRegular8* m_label = nullptr;
  const MacroControlParameter* m_parameter = nullptr;
  void drawNonLEDTargets(FrameBuffer& fb);
};
