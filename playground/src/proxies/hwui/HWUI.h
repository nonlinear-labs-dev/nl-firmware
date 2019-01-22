#pragma once

#include <libundo/undo/Scope.h>
#include <playground.h>
#include <proxies/hwui/base-unit/BaseUnit.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/FineButton.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <tools/Expiration.h>
#include <tools/Signal.h>
#include <io/network/WebSocketSession.h>
#include <array>
#include <memory>

class Application;
class UsageMode;

class HWUI
{
 public:
  HWUI();
  virtual ~HWUI();

  void init();

  void indicateBlockingMainThread();

  void undoableSetFocusAndMode(UNDO::Transaction *transaction, FocusAndMode focusAndMode);
  void undoableSetFocusAndMode(FocusAndMode focusAndMode);
  void setFocusAndMode(FocusAndMode focusAndMode);
  FocusAndMode getFocusAndMode() const;

  void freezeFocusAndMode();
  void thawFocusAndMode();

  PanelUnit &getPanelUnit();
  const PanelUnit &getPanelUnit() const;

  BaseUnit &getBaseUnit();
  const BaseUnit &getBaseUnit() const;

  void testDisplays();

  ButtonModifiers getButtonModifiers() const;
  bool isResolutionFine() const;
  void unsetFineMode();
  bool isModifierSet(ButtonModifier m) const;
  bool getButtonState(uint16_t buttonId) const;

  sigc::connection onModifiersChanged(slot<void, ButtonModifiers> cb);
  sigc::connection connectToBlinkTimer(slot<void, int> cb);
  void deInit();

 private:
  void onButtonMessage(WebSocketSession::tMessage msg);
  void onButtonPressed(int buttonID, bool state);
  void onKeyboardLineRead(Glib::RefPtr<Gio::AsyncResult> &res);

  void addModifier(ButtonModifier i);
  void removeModifier(ButtonModifier i);
  void setModifiers(ButtonModifiers m);

  bool onBlinkTimeout();
  void setupFocusAndMode();

  void setModifiers(int buttonID, bool state);
  bool detectAffengriff(int buttonID, bool state);
  bool isFineAllowed();

  FocusAndMode restrictFocusAndMode(FocusAndMode in) const;

  PanelUnit m_panelUnit;
  BaseUnit m_baseUnit;

  RefPtr<Gio::Cancellable> m_readersCancel;
  RefPtr<Gio::DataInputStream> m_keyboardInput;

  FineButton m_fineButton;
  ButtonModifiers m_modifiers;

  sigc::connection m_blinkTimerConnection;
  Signal<void, ButtonModifiers> m_modifersChanged;
  Signal<void, int> m_blinkTimer;
  std::array<bool, NUM_BUTTONS> m_buttonStates;

  int m_affengriffState = 0;

  FocusAndMode m_focusAndMode;

  int m_blinkCount;
  Expiration m_switchOffBlockingMainThreadIndicator;

  bool m_focusAndModeFrozen = false;
};
