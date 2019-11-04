#pragma once
#include <proxies/hwui/controls/SymbolLabel.h>

class CurrentVoiceGroupSelectionIndicator : public SymbolLabel
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