#pragma once

#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/ButtonParameterMapping.h>
#include <proxies/hwui/panel-unit/UndoButtonsStateMachine.h>
#include <proxies/hwui/UsageMode.h>
#include <ParameterId.h>
#include <sigc++/connection.h>
#include <array>
#include <list>
#include <vector>
#include <proxies/hwui/TwoStateLED.h>

class PresetBank;
class Parameter;
class Layout;
class Layout;
class BOLED;
class MacroControlAssignmentStateMachine;

class PanelUnitParameterEditMode : public UsageMode
{
  typedef UsageMode super;

 public:
  PanelUnitParameterEditMode();
  ~PanelUnitParameterEditMode() override;

  void setup() override;
  void setupFocusAndMode(FocusAndMode focusAndMode) override;
  void bruteForceUpdateLeds() override;

  Buttons findButtonForParameter(Parameter *param) const;
  std::list<int> getButtonAssignments(Buttons button) const;
  std::list<int> getButtonAssignments(Buttons button, SoundType type) const;

  static const int NUM_LEDS = 96;

 protected:
  ButtonParameterMapping &getMappings();

 private:
  typedef std::array<TwoStateLED::LedState, NUM_LEDS> tLedStates;

  void onParamSelectionChanged(Parameter *oldParam, Parameter *newParam);

  bool tryParameterToggleOnMacroControl(const std::vector<gint32> &ids, Parameter *selParam);

  tAction createParameterSelectAction(const std::vector<gint32> &toggleAudioIDs);

  bool toggleParameterSelection(const std::vector<gint32> &ids, bool state);
  bool setParameterSelection(const ParameterId &audioID, bool state);

  bool isShowingParameterScreen() const;
  bool switchToNormalModeInCurrentParameterLayout();

  void collectLedStates(tLedStates &states, ParameterId selectedParameterID);

  void letTargetsBlink(Parameter *selParam, tLedStates &states);
  void letMacroControlTargetsBlink(tLedStates &states);
  void letOscAShaperABlink(const std::vector<int> &targets, tLedStates &states);
  void letOscBShaperBBlink(const std::vector<int> &targets, tLedStates &states);
  void letOtherTargetsBlink(const std::vector<int> &targets, tLedStates &states);
  void letReverbBlink(const std::vector<int> &targets, tLedStates &states);
  bool isSignalFlowingThrough(const Parameter *p) const;
  void setLedStates(const tLedStates &states);

  const BOLED &getBoled() const;
  BOLED &getBoled();
  bool handleMacroControlButton(bool state, int mcParamId);

  MacroControlAssignmentStateMachine &getMacroControlAssignmentStateMachine();

  ButtonParameterMapping m_mappings;
  UndoButtonStateMachine m_undoStateMachine;

  sigc::connection m_connectionToMacroControl;
};
