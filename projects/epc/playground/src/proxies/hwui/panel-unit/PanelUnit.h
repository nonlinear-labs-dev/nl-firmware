#pragma once

#include "EditPanel.h"
#include "proxies/hwui/HardwareUserInterfaceUnit.h"
#include "MacroControlAssignmentStateMachine.h"

class Application;
class Parameter;
class TwoStateLED;
class LayoutFolderMonitor;

class PanelUnit : public HardwareUserInterfaceUnit, public sigc::trackable
{
 private:
  typedef HardwareUserInterfaceUnit super;

 public:
  typedef std::shared_ptr<TwoStateLED> tLed;

  PanelUnit(Settings &settings, Oleds &oleds, LayoutFolderMonitor *mon);
  ~PanelUnit() override;

  bool onButtonPressed(Buttons buttonID, ButtonModifiers modifiers, bool state) override;

  void onTimeout();
  tLed getLED(Buttons id);

  const std::vector<tLed> &getLeds();
  void init();
  void turnLedsOff();

  const EditPanel &getEditPanel() const;
  EditPanel &getEditPanel();
  MacroControlAssignmentStateMachine &getMacroControlAssignmentStateMachine();

 protected:
  void setupFocusAndMode(FocusAndMode focusAndMode) override;

 private:
  void installUsageMode(FocusAndMode focusAndMode);
  void initModulateableParameterLayout(Layout *l);
  void onBBBBConnected();
  ParameterId choseHWBestSourceForMC(const ParameterId &mcParamId) const;

  EditPanel m_editPanel;
  std::vector<tLed> m_leds;
  MacroControlAssignmentStateMachine m_macroControlAssignmentStateMachine;

  std::shared_ptr<UsageMode> m_overlayUsageMode;

  sigc::connection m_signalInitializeInstalledLayoutOnce;
};
