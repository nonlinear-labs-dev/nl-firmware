#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ArrowIncrementDecrementOverlay.h>

class Parameter;

class MonoParameterOverlay : public ArrowIncrementDecrementOverlay
{
 public:
  MonoParameterOverlay(const Rect& r, const Parameter* parameter);

  void onLeft(bool down) override;
  void onRight(bool down) override;
  void onUp(bool down) override;
  void onDown(bool down) override;
  void onCommit(bool down) override;

 private:
  const Parameter* m_parameter;
};
