#pragma once

#include <proxies/hwui/controls/Button.h>

class NeverHighlitButton : public Button
{
 public:
  using Button::Button;
  void setHighlight(bool isHighlight) override;
};
