#pragma once
#include <proxies/hwui/controls/LabelRegular8.h>

class CurrentVoiceGroupSelectionIndicator : public LabelRegular8
{
 public:
  explicit CurrentVoiceGroupSelectionIndicator(const Rect& r);
  void focusChanged();
};