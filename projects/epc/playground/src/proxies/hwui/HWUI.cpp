#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <http/UndoScope.h>
#include <libundo/undo/SwapData.h>
#include <libundo/undo/Transaction.h>
#include <libundo/undo/TransactionCreationScope.h>
#include <cmath>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/value/QuantizedValue.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/base-unit/PlayPanel.h>
#include <proxies/hwui/base-unit/soled/SOLED.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/debug-oled/DebugOLED.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <tools/Signal.h>
#include <xml/FileOutStream.h>
#include <groups/HardwareSourcesGroup.h>
#include <tools/PerformanceTimer.h>
#include <proxies/hwui/descriptive-layouts/LayoutFolderMonitor.h>
#include <nltools/messaging/Message.h>
#include <device-settings/LayoutMode.h>
#include <xml/StandardOutStream.h>
#include <serialization/EditBufferSerializer.h>
#include <iostream>
#include <giomm.h>
#include <proxies/hwui/FrameBuffer.h>
#include "UsageMode.h"
#include "use-cases/SettingsUseCases.h"
#include <device-info/AftertouchCalibratedStatus.h>
#include <device-info/DeviceInformation.h>
#include "use-cases/EditBufferUseCases.h"
#include "device-settings/ScreenSaverTimeoutSetting.h"
#include "proxies/hwui/panel-unit/boled/recorder/DoYouWantToStopRecorderPlaybackLayout.h"
#include <Options.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/HardwareFeatures.h>
#include <malloc.h>

HWUI::HWUI(Settings &settings, RecorderManager &recorderManager)
    : m_layoutFolderMonitor(std::make_unique<LayoutFolderMonitor>())
    , m_panelUnit { settings, m_oleds, m_layoutFolderMonitor.get() }
    , m_baseUnit { settings, m_oleds }
    , m_readersCancel(Gio::Cancellable::create())
    , m_buttonStates { false }
    , m_blinkCount(0)
    , m_switchOffBlockingMainThreadIndicator(Application::get().getMainContext())
    , m_settings { settings }
    , m_famSetting(*settings.getSetting<FocusAndModeSetting>())
{
  if(isatty(fileno(stdin)) && Options::s_acceptanceTests == false)
  {
    m_keyboardInput = Gio::DataInputStream::create(Gio::UnixInputStream::create(0, true));
    m_keyboardInput->read_line_async(mem_fun(this, &HWUI::onKeyboardLineRead), m_readersCancel);
  }

  nltools::msg::receive<nltools::msg::ButtonChangedMessage>(nltools::msg::EndPoint::Playground,
                                                            sigc::mem_fun(this, &HWUI::onButtonMessage));

  recorderManager.subscribeToNotifyNoRecorderUIsLeftAndStillPlaying(
      [this] { getPanelUnit().getEditPanel().getBoled().setOverlay(new DoYouWantToStopRecorderPlaybackLayout()); });
}

HWUI::~HWUI()
{
  deInit();
  m_blinkTimerConnection.disconnect();
  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);
  m_readersCancel->cancel();
}

void HWUI::deInit()
{
  m_oleds.deInit();
}

void HWUI::onButtonMessage(const nltools::msg::ButtonChangedMessage &msg)
{
  onButtonPressed(static_cast<Buttons>(msg.buttonId), msg.pressed);
}

void HWUI::init()
{
  m_layoutFolderMonitor->start();
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  m_panelUnit.init();
  m_baseUnit.init();

  m_editBufferParameterReselectionConnection
      = eb->onParameterReselected(sigc::mem_fun(this, &HWUI::onParameterReselection));

  m_editBufferParameterSelectionConnection
      = eb->onSelectionChanged(sigc::mem_fun(this, &HWUI::onParameterSelection), std::nullopt);

  m_rotaryChangedConnection = getPanelUnit().getEditPanel().getKnob().onRotaryChanged(
      sigc::hide(sigc::mem_fun(this, &HWUI::onRotaryChanged)));
  m_oleds.syncRedraw();
}

void HWUI::onRotaryChanged()
{
  m_rotaryTurned.deferedSend();
}

void HWUI::indicateBlockingMainThread()
{
  m_switchOffBlockingMainThreadIndicator.setCallback([=]() { m_baseUnit.indicateBlockingMainThread(false); });

  m_switchOffBlockingMainThreadIndicator.refresh(std::chrono::seconds(5));
  m_baseUnit.indicateBlockingMainThread(true);
}

void HWUI::onKeyboardLineRead(Glib::RefPtr<Gio::AsyncResult> &res)
{
  std::string line;

  if(m_keyboardInput->read_line_finish(res, line))
  {
    if(!line.empty())
    {
      if(line == "mem")
      {
        nltools::Log::memUsage(nltools::Log::Level::Error, __PRETTY_FUNCTION__, mallinfo().uordblks);
      }
      else if(line == "r")
      {
        m_layoutFolderMonitor->bruteForce();
      }
      else if(line == "t")
      {
        onButtonPressed(Buttons::BUTTON_SETUP, true);
      }
      else if(line == "!t")
      {
        onButtonPressed(Buttons::BUTTON_SETUP, false);
      }
      else if(line == "s")
      {
        onButtonPressed(Buttons::BUTTON_SHIFT, true);
      }
      else if(line == "!s")
      {
        onButtonPressed(Buttons::BUTTON_SHIFT, false);
      }
      else if(line == "a")
      {
        onButtonPressed(Buttons::BUTTON_A, true);
      }
      else if(line == "!a")
      {
        onButtonPressed(Buttons::BUTTON_A, false);
      }
      else if(line == "b")
      {
        onButtonPressed(Buttons::BUTTON_B, true);
      }
      else if(line == "!b")
      {
        onButtonPressed(Buttons::BUTTON_B, false);
      }
      else if(line == "c")
      {
        onButtonPressed(Buttons::BUTTON_C, true);
      }
      else if(line == "!c")
      {
        onButtonPressed(Buttons::BUTTON_C, false);
      }
      else if(line == "d")
      {
        onButtonPressed(Buttons::BUTTON_D, true);
      }
      else if(line == "!d")
      {
        onButtonPressed(Buttons::BUTTON_D, false);
      }
      else if(line == "e")
      {
        onButtonPressed(Buttons::BUTTON_ENTER, true);
      }
      else if(line == "!e")
      {
        onButtonPressed(Buttons::BUTTON_ENTER, false);
      }
      else if(line == "i")
      {
        onButtonPressed(Buttons::BUTTON_INFO, true);
        onButtonPressed(Buttons::BUTTON_INFO, false);
      }
      else if(line == "!666")
      {
        onButtonPressed(Buttons::BUTTON_UNDO, true);
        onButtonPressed(Buttons::BUTTON_REDO, true);
        onButtonPressed(Buttons::BUTTON_UNDO, false);
        onButtonPressed(Buttons::BUTTON_REDO, false);
      }
      else if(line == "stress-undo-s")
      {
        Application::get().getPresetManager()->stress(1000);
      }
      else if(line == "stress-undo-m")
      {
        Application::get().getPresetManager()->stress(10000);
      }
      else if(line == "stress-undo-l")
      {
        Application::get().getPresetManager()->stress(100000);
      }
      else if(line == "stress-change-all")
      {
        Application::get().getPresetManager()->stressAllParams(500);
      }
      else if(line == "stress-pm")
      {
        Application::get().getPresetManager()->stressLoad(1000);
      }
      else if(line == "dump-editbuffer")
      {
        auto eb = Application::get().getPresetManager()->getEditBuffer();
        auto wtf = std::shared_ptr<std::ostream>(&std::cout, [](void *) {});
        StandardOutStream stream(wtf);
        XmlWriter writer(stream);
        eb->writeDocument(writer, 0);

        EditBufferSerializer ebs(eb, {});
        ebs.write(writer);
      }
      else if(line == "undo-performance-compare")
      {
        Application::get().stopWatchDog();

        for(int steps = 0; steps < 50; steps++)
        {
          unsigned long long totalTraverse = 0;
          long avgusTraverse = 0;
          for(int i = 1; i < 101; i++)
          {
            auto traverse = Application::get().getPresetManager()->getUndoScope().getRootTransaction()->traverseTree();
            totalTraverse += traverse;
          }
          avgusTraverse = static_cast<long>(totalTraverse / 100);
          DebugLevel::warning("Count: ~", steps * 1000, "Transactions Traverse avg:", avgusTraverse / 1000, "\bms");
          Application::get().getPresetManager()->stressBlocking(1000);
        }
        Application::get().runWatchDog();
      }
      else if(line == "mod-all")
      {
        EditBufferUseCases ebUseCases(*Application::get().getPresetManager()->getEditBuffer());
        ebUseCases.setModulationSourceOfAll(MacroControls::MC1);
      }
      else if(line == "inc-all-fine")
      {
        Application::get().getPresetManager()->incAllParamsFine();
      }
      else if(line == "epc-info")
      {
        auto hwInfo = Application::get().getHardwareFeatures();
        nltools::Log::error("epc model:", toString(hwInfo->getModel()));
        nltools::Log::error("has epc wifi:", hwInfo->hasEPCWiFi());
      }
      else if(line.at(0) == '!')
      {
        onButtonPressed(Buttons::BUTTON_SHIFT, true);
        m_panelUnit.getEditPanel().getKnob().fake(1);
        onButtonPressed(Buttons::BUTTON_SHIFT, false);
      }
      else if(line.at(0) == '@')
      {
        onButtonPressed(Buttons::BUTTON_SHIFT, true);
        m_panelUnit.getEditPanel().getKnob().fake(-1);
        onButtonPressed(Buttons::BUTTON_SHIFT, false);
      }
      else if(line.at(0) == '+')
      {
        float f = line.size();
        float sign = f < 0 ? -1 : 1;
        f = powf(f, 1.5f) * sign;
        auto c = static_cast<signed char>(roundf(f));
        m_panelUnit.getEditPanel().getKnob().fake(c);
      }
      else if(line.at(0) == '-')
      {
        float f = line.size();
        f = -f;
        float sign = f < 0 ? -1 : 1;
        f = powf(fabsf(f), 1.5f) * sign;
        auto c = static_cast<signed char>(roundf(f));
        m_panelUnit.getEditPanel().getKnob().fake(c);
      }
      else if(line == "atc")
      {
        auto dev = Application::get().getDeviceInformation();
        dev->getItem<AftertouchCalibratedStatus>()->toggle();
        nltools::Log::error("ATC ist nun:", dev->getItem<AftertouchCalibratedStatus>()->get());
      }
      else
      {
        try
        {
          auto i = (Buttons) std::stoi(line);
          if(i < Buttons::NUM_BUTTONS)
          {
            if(line.back() == 'u')
            {
              onButtonPressed(i, false);
            }
            else if(line.back() == 'd')
            {
              onButtonPressed(i, true);
            }
            else
            {
              onButtonPressed(i, true);
              onButtonPressed(i, false);
            }
          }
          else
          {
            DebugLevel::error("Number bigger than ", m_buttonStates.size(), __PRETTY_FUNCTION__, __LINE__);
          }
        }
        catch(...)
        {
          DebugLevel::error("Exception caught in", __PRETTY_FUNCTION__, __LINE__);
        }
      }
    }
  }

  m_keyboardInput->read_line_async(mem_fun(this, &HWUI::onKeyboardLineRead), m_readersCancel);
}

PanelUnit &HWUI::getPanelUnit()
{
  return m_panelUnit;
}

const PanelUnit &HWUI::getPanelUnit() const
{
  return m_panelUnit;
}

BaseUnit &HWUI::getBaseUnit()
{
  return m_baseUnit;
}

void HWUI::onButtonPressed(Buttons buttonID, bool state)
{
  if(buttonID == Buttons::BUTTON_ENCODER)
    buttonID = Buttons::BUTTON_ENTER;

  m_buttonStates[(int) buttonID] = state;

  setModifiers(buttonID, state);
  m_buttonPressed.deferedSend(buttonID, state);

  if(!detectAffengriff(buttonID, state))
  {
    if(!m_baseUnit.onButtonPressed(buttonID, m_modifiers, state))
    {
      if(!m_panelUnit.onButtonPressed(buttonID, m_modifiers, state))
      {
        if(buttonID == Buttons::BUTTON_SETUP && state)
        {
          SettingsUseCases useCases(m_settings);
          if(m_famSetting.getState().focus == UIFocus::Setup)
          {
            useCases.setFocusAndMode(m_famSetting.getOldState());
          }
          else
          {
            useCases.setFocusAndMode({ UIFocus::Setup, UIMode::Select });
          }
        }
      }
    }
  }
}

void HWUI::setModifiers(Buttons buttonID, bool state)
{
  if(buttonID == Buttons::BUTTON_SHIFT)
  {
    if(state)
    {
      addModifier(ButtonModifier::SHIFT);
    }
    else
    {
      removeModifier(ButtonModifier::SHIFT);
    }
  }

  if(buttonID == Buttons::BUTTON_FINE)
  {
    if(isFineAllowed())
    {
      m_fineButton.setShiftedWhilePressDown(isModifierSet(ButtonModifier::SHIFT));
      m_fineButton.setButtonState(state);
    }
  }
  if(m_fineButton.getModifierState())
  {
    addModifier(ButtonModifier::FINE);
  }
  else
  {
    removeModifier(ButtonModifier::FINE);
  }
}

bool HWUI::isFineAllowed()
{
  auto uiFocus = m_famSetting.getState().focus;
  return ((uiFocus == UIFocus::Parameters || uiFocus == UIFocus::Sound) && m_currentParameterIsFineAllowed)
      || uiFocus == UIFocus::Setup;
}

bool HWUI::detectAffengriff(Buttons buttonID, bool state)
{
  if(!state)
  {
    m_affengriffState = 0;
    return false;
  }

  if(m_affengriffState == 0 && (buttonID == Buttons::BUTTON_SHIFT))
  {
    m_affengriffState = 1;
  }
  else if(m_affengriffState == 1 && (buttonID == Buttons::BUTTON_SETUP))
  {
    m_affengriffState = 2;
  }
  else
  {
    m_affengriffState = 0;
  }
  return false;
}

void HWUI::unsetFineMode()
{
  if(m_fineButton.onlyTemporary())
  {
    m_fineButton.setState(TOGGLED_OFF);
    removeModifier(ButtonModifier::FINE);
  }
}

ButtonModifiers HWUI::getButtonModifiers() const
{
  return m_modifiers;
}

bool HWUI::isModifierSet(ButtonModifier m) const
{
  return m_modifiers[m];
}

bool HWUI::isResolutionFine() const
{
  return isModifierSet(ButtonModifier::FINE);
}

void HWUI::setModifiers(ButtonModifiers m)
{
  if(m != m_modifiers)
  {
    m_modifiers = m;
    m_modifersChanged.send(m_modifiers);
  }
}

void HWUI::addModifier(ButtonModifier i)
{
  auto cp = m_modifiers;
  cp.set(i);
  setModifiers(cp);
}

void HWUI::removeModifier(ButtonModifier i)
{
  auto cp = m_modifiers;
  cp.reset(i);
  setModifiers(cp);
}

sigc::connection HWUI::onModifiersChanged(const sigc::slot<void, ButtonModifiers> &cb)
{
  return m_modifersChanged.connectAndInit(cb, m_modifiers);
}

sigc::connection HWUI::connectToBlinkTimer(const sigc::slot<void, int> &cb)
{
  if(m_blinkTimer.empty())
  {
    m_blinkTimerConnection.disconnect();

    m_blinkTimerConnection = Application::get().getMainContext()->signal_timeout().connect(
        sigc::mem_fun(this, &HWUI::onBlinkTimeout), 500);
    m_blinkCount = 1;
  }

  return m_blinkTimer.connectAndInit(cb, m_blinkCount);
}

bool HWUI::onBlinkTimeout()
{
  m_blinkTimer.send(++m_blinkCount);
  return true;
}

std::string HWUI::exportBoled()
{
  std::string name
      = "/tmp/boled-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".png";
  exportOled(0, 0, 256, 64, name);
  return name;
}

std::string HWUI::exportSoled()
{
  auto name = "/tmp/soled-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".png";
  exportOled(0, 64, 128, 32, name);
  return name;
}

void HWUI::exportOled(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::string &fileName) const
{
  typedef std::tuple<uint8_t, uint8_t, uint8_t> RGB;
  static std::map<uint8_t, RGB> colorMap
      = { { 0x00, std::make_tuple(0, 0, 0) },       { 0x02, std::make_tuple(73, 73, 73) },
          { 0x05, std::make_tuple(110, 110, 110) }, { 0x06, std::make_tuple(146, 146, 146) },
          { 0x0A, std::make_tuple(183, 183, 183) }, { 0x0B, std::make_tuple(219, 219, 219) },
          { 0x0F, std::make_tuple(255, 255, 255) } };

  constexpr auto frameBufferDimX = 256;
  constexpr auto frameBufferDimY = 96;

  auto &fb = m_oleds.getFrameBuffer();
  auto &buffer = fb.getBackBuffer();
  png::image<png::rgb_pixel> boledFile(w, h);

  for(png::uint_32 iy = 0; iy < h; ++iy)
  {
    for(png::uint_32 ix = 0; ix < w; ++ix)
    {
      int idx = (iy + y) * frameBufferDimX + (ix + x);
      auto pixel = buffer[idx];
      try
      {
        auto rgb = colorMap.at(pixel);
        boledFile[iy][ix] = png::rgb_pixel(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
      }
      catch(...)
      {
        boledFile[iy][ix] = png::rgb_pixel();
      }
    }
  }

  boledFile.write(fileName);
}

void HWUI::onParameterReselection(Parameter *parameter)
{
  SettingsUseCases useCases(m_settings);

  if(m_famSetting.getState().mode == UIMode::Info)
    useCases.setFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Info));
  else
    useCases.setFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Select));
}

void HWUI::onParameterSelection(Parameter *oldParameter, Parameter *newParameter)
{
  m_currentParameterIsFineAllowed = newParameter->isFineAllowed();

  if(!m_currentParameterIsFineAllowed)
    m_fineButton.setState(FineButtonStates::TOGGLED_OFF);
  unsetFineMode();

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(!eb->isParameterFocusLocked())
  {
    SettingsUseCases useCases(m_settings);
    if(m_famSetting.getState().focus == UIFocus::Sound)
    {
      if(oldParameter->getID() != newParameter->getID())
      {
        useCases.setFocusAndMode(FocusAndMode { UIFocus::Parameters });
      }
    }
    else
    {
      useCases.setFocusAndMode(FocusAndMode { UIFocus::Parameters });
    }
  }
}

Oleds &HWUI::getOleds()
{
  return m_oleds;
}

sigc::connection HWUI::onButtonPressed(const sigc::slot<void, Buttons, bool> &cb)
{
  return m_buttonPressed.connect(cb);
}

sigc::connection HWUI::onRotaryTurned(const sigc::slot<void> &cb)
{
  return m_rotaryTurned.connect(cb);
}

void HWUI::toggleFine()
{
  if(getButtonModifiers()[ButtonModifier::FINE])
  {
    removeModifier(ButtonModifier::FINE);
  }
  else
  {
    addModifier(ButtonModifier::FINE);
  }
}
