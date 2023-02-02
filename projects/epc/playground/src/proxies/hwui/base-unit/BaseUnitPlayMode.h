#pragma once

#include "proxies/hwui/UsageMode.h"
#include "NoteShiftStateMachine.h"
#include <proxies/hwui/ShortVsLongPress.h>

class BaseUnitUIDetail;

class BaseUnitPlayMode : public UsageMode
{
 private:
  typedef UsageMode super;

 public:
  BaseUnitPlayMode();
  void setup() override;

 protected:
  virtual void toggleTouchBehaviour();

 private:
  void setupBaseUnitUIModeButton();
  void setupBaseUnitMinusButton();
  void setupBaseUnitPlusButton();
  static void modeButtonShortPress();
  static void modeButtonLongPress();
  void funcButtonShortPress();
  static void funcButtonLongPress();

  static void toggleRibbonSelection();
  static void toggleRibbonMode();

  std::unordered_map<Buttons, bool> m_buttonStates;
  NoteShiftStateMachine m_noteShiftState;
  ShortVsLongPress m_modeButtonHandler;
  ShortVsLongPress m_funcButtonHandler;

  bool checkPanicAffenGriff(Buttons b, bool state);

  static BaseUnitUIDetail* getDetailSetting();
};
