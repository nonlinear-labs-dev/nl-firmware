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
  PanelUnit();
  virtual ~PanelUnit();

  typedef std::shared_ptr<TwoStateLED> tLed;

  void setupFocusAndMode();
  virtual void setupFocusAndMode(FocusAndMode focusAndMode) override;
  void onTimeout();

  tLed getLED(Buttons id);
  const std::vector<tLed>& getLeds();
  EditPanel &getEditPanel();
  const EditPanel &getEditPanel() const;

  virtual bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state) override;
  void init();
  void turnLedsOff();

  MacroControlAssignmentStateMachine &getMacroControlAssignmentStateMachine();

 private:
  void onScreenSaverStateChanged(bool state);
  void installUsageMode(FocusAndMode focusAndMode);
  ParameterId choseHWBestSourceForMC(const ParameterId &mcParamId) const;
  void onBBBBConnected();

  EditPanel m_editPanel;
  std::vector<tLed> m_leds;
  MacroControlAssignmentStateMachine m_macroControlAssignmentStateMachine;
};
