#pragma once
#include <proxies/hwui/controls/LabelRegular8.h>

class CurrentVoiceGroupSelectionIndicator : public LabelRegular8
{
 public:
  explicit CurrentVoiceGroupSelectionIndicator(const Rect& r);
  void focusChanged();

 private:
  /*
      split.a.01	E0F0
      split.a.02	E0F1
      split.a.03	E0F2
      layer.a.01	E0F3
      layer.a.02	E0F4
      layer.a.03	E0F5
   */
};