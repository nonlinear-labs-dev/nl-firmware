#pragma once
#include <proxies/hwui/controls/Overlay.h>
#include <proxies/hwui/panel-unit/ButtonReceiver.h>
#include <parameters/ModulateableParameter.h>

class ModAspectRecallOverlay : public Overlay, public ButtonReceiver
{
 public:
  ModAspectRecallOverlay(const Rect& r, ModulateableParameter* modP);
  bool onButton(Buttons i, bool down, ButtonModifiers mod) override;

  enum Mode {
    MC_POS,
    MC_SEL,
    MC_AMT
  };

 private:
  void installMode(Mode m);

  Mode m_mode;
  ModulateableParameter* m_modParam;
};