#pragma once

#include <playground.h>
#include <proxies/hwui/base-unit/BaseUnit.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/FineButton.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <nltools/threading/Expiration.h>
#include <tools/Signal.h>
#include <tools/DelayedJob.h>
#include <array>
#include <memory>
#include <glibmm/refptr.h>
#include <png++/image.hpp>
#include <tools/ScopedGuard.h>
#include "Oleds.h"

class Application;
class UsageMode;
class Settings;
class LayoutFolderMonitor;

namespace UNDO
{
  class Transaction;
}

namespace nltools::msg
{
  struct ButtonChangedMessage;
}

class PresetPartSelection;
class SplashLayout;

class HWUI
{
 public:
  HWUI(Settings &settings);
  virtual ~HWUI();
  void init();
  void indicateBlockingMainThread();

  PanelUnit &getPanelUnit();
  const PanelUnit &getPanelUnit() const;

  BaseUnit &getBaseUnit();

  ButtonModifiers getButtonModifiers() const;
  bool isResolutionFine() const;
  void unsetFineMode();
  bool isModifierSet(ButtonModifier m) const;

  sigc::connection onModifiersChanged(const sigc::slot<void, ButtonModifiers> &cb);
  sigc::connection connectToBlinkTimer(const sigc::slot<void, int> &cb);
  void deInit();

  std::string exportSoled();
  std::string exportBoled();

  Oleds &getOleds();

  //SplashScreen
  void startSplash();
  void finishSplash();
  void setSplashStatus(const std::string &msg);
  void addSplashStatus(const std::string &msg);
  void registerSplash(SplashLayout *l);
  void unregisterSplash(SplashLayout *l);

 private:
  void exportOled(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::string &fileName) const;

  void onFocusAndModeChanged(const Setting *s);
  void onPresetLoaded();
  void onEditBufferSoundTypeChanged(SoundType type);
  void undoableUpdateParameterSelection(UNDO::Transaction *transaction);
  void onButtonMessage(const nltools::msg::ButtonChangedMessage &msg);
  void onButtonPressed(Buttons buttonID, bool state);

  void onKeyboardLineRead(Glib::RefPtr<Gio::AsyncResult> &res);

  void addModifier(ButtonModifier i);
  void removeModifier(ButtonModifier i);
  void setModifiers(ButtonModifiers m);

  bool onBlinkTimeout();

  void setModifiers(Buttons buttonID, bool state);
  bool detectAffengriff(Buttons buttonID, bool state);
  bool isFineAllowed();

  void onParameterReselection(Parameter *parameter);
  void onParameterSelection(Parameter *oldParameter, Parameter *newParameter);


  Oleds m_oleds;

  sigc::connection m_editBufferSoundTypeConnection;
  sigc::connection m_editBufferPresetLoadedConnection;
  sigc::connection m_rotaryChangedConnection;
  sigc::connection m_focusAndModeConnection;
  sigc::connection m_blinkTimerConnection;
  sigc::connection m_editBufferParameterReselectionConnection;
  sigc::connection m_editBufferParameterSelectionConnection;

  void onRotaryChanged();
  Signal<void> m_inputSignal;

  std::unique_ptr<LayoutFolderMonitor> m_layoutFolderMonitor;
  PanelUnit m_panelUnit;
  BaseUnit m_baseUnit;

  Glib::RefPtr<Gio::Cancellable> m_readersCancel;
  Glib::RefPtr<Gio::DataInputStream> m_keyboardInput;

  FineButton m_fineButton;
  ButtonModifiers m_modifiers;

  Signal<void, ButtonModifiers> m_modifersChanged;
  Signal<void, int> m_blinkTimer;
  Signal<void, FocusAndMode> m_signalFocusAndMode;

  std::array<bool, (size_t) Buttons::NUM_BUTTONS> m_buttonStates;
  int m_affengriffState = 0;
  int m_blinkCount;
  Expiration m_switchOffBlockingMainThreadIndicator;
  ScopedGuard m_parameterFocusLock;
  Settings &m_settings;
  FocusAndModeSetting &m_famSetting;
  SplashLayout *m_splashLayout = nullptr;

  bool m_currentParameterIsFineAllowed = false;
};
