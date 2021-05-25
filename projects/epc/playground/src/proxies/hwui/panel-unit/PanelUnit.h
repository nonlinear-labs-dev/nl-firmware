#pragma once

#include "EditPanel.h"
#include "proxies/hwui/HardwareUserInterfaceUnit.h"
#include "MacroControlAssignmentStateMachine.h"

class Application;
class Parameter;
class TwoStateLED;

class PanelUnit : public HardwareUserInterfaceUnit, public sigc::trackable
{
 private:
  typedef HardwareUserInterfaceUnit super;

 public:
  typedef std::shared_ptr<TwoStateLED> tLed;

  PanelUnit();
  ~PanelUnit() override;

  void setupFocusAndMode(FocusAndMode focusAndMode) override;
  bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state) override;

  void setupFocusAndMode();
  void onTimeout();
  tLed getLED(Buttons id);

  const std::vector<tLed> &getLeds();
  void init();
  void turnLedsOff();

  const EditPanel &getEditPanel() const;
  EditPanel &getEditPanel();
  MacroControlAssignmentStateMachine &getMacroControlAssignmentStateMachine();

 private:
  void onScreenSaverStateChanged(bool state);
  void installUsageMode(FocusAndMode focusAndMode);
  void initModulateableParameterLayout(Layout *l);
  void onBBBBConnected();
  ParameterId choseHWBestSourceForMC(const ParameterId &mcParamId) const;

  void setOverlayUsageMode(UsageMode *u);
  void resetOverlayUsageMode();

  EditPanel m_editPanel;
  std::vector<tLed> m_leds;
  MacroControlAssignmentStateMachine m_macroControlAssignmentStateMachine;
  std::shared_ptr<UsageMode> m_overlayUsageMode;

  std::shared_ptr<UsageMode> m_stashedUsageMode = nullptr;
  sigc::connection m_signalInitializeInstalledLayoutOnce;
};
