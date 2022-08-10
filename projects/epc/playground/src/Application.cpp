#include "Application.h"
#include "device-info/DeviceInformation.h"
#include "device-settings/DebugLevel.h"
#include "device-settings/Settings.h"
#include <device-settings/BlockingMainThreadIndication.h>
#include <fcntl.h>
#include "http/HTTPServer.h"
#include "http/UndoScope.h"
#include "Options.h"
#include "presets/PresetManager.h"
#include "profiling/Profiler.h"
#include "proxies/hwui/HWUI.h"
#include "proxies/playcontroller/PlaycontrollerProxy.h"
#include "proxies/audio-engine/AudioEngineProxy.h"
#include <tools/WatchDog.h>
#include <unistd.h>
#include <clipboard/Clipboard.h>
#include <cassert>
#include <proxies/hwui/debug-oled/DebugLayout.h>
#include <tools/ExceptionTools.h>
#include <nltools/messaging/Messaging.h>
#include <presets/EditBuffer.h>
#include <giomm.h>
#include <proxies/usb/USBChangeListener.h>
#include <http/WebUISupport.h>
#include <presets/PresetManagerActions.h>
#include <presets/BankActions.h>
#include <presets/EditBufferActions.h>
#include <use-cases/SettingsUseCases.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <nltools/system/SpawnAsyncCommandLine.h>

using namespace std::chrono_literals;

Application *Application::theApp = nullptr;

void setupMessaging(const Options *options, Glib::RefPtr<Glib::MainContext> pContext)
{
  using namespace nltools::msg;

  const auto &bbbb = options->getBBBB();
  const auto &ae = options->getAudioEngineHost();
  const auto &midi = options->getMidiBridge();

  Configuration conf;
  conf.mainContext = pContext;

#ifdef _DEVELOPMENT_PC
  conf.offerEndpoints = { { EndPoint::Playground }, { EndPoint::TestEndPoint } };
#else
  conf.offerEndpoints = { EndPoint::Playground };
#endif
  conf.useEndpoints = {
    { EndPoint::Playcontroller, bbbb }, { EndPoint::Oled, bbbb },      { EndPoint::ExternalMidiOverIPBridge, midi },
#ifdef _DEVELOPMENT_PC
    { EndPoint::TestEndPoint },
#endif
    { EndPoint::PanelLed, bbbb },       { EndPoint::RibbonLed, bbbb }, { EndPoint::AudioEngine, ae },
    { EndPoint::BeagleBone, bbbb }
  };
  nltools::msg::init(conf);
}

std::unique_ptr<Options> Application::initStatic(Application *app, std::unique_ptr<Options> options)
{
  g_main_context_push_thread_default(app->m_theMainContext->gobj());
  theApp = app;
  setupMessaging(options.get(), app->m_theMainContext);
  return options;
}

void quitApp(int sig)
{
  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__, sig);
  Application::get().quit();
}

Glib::RefPtr<Glib::MainContext> createMainContext()
{
  if(Options::s_acceptanceTests)
  {
    return Glib::MainContext::create();
  }
  else
  {
    return Glib::MainContext::get_default();
  }
}

Application::Application(int numArgs, char **argv)
    : m_theMainContext(createMainContext())
    , m_options(initStatic(this, std::make_unique<Options>(numArgs, argv)))
    , m_theMainLoop(Glib::MainLoop::create(m_theMainContext))
    , m_http(new HTTPServer())
    , m_settings(new Settings(m_options->getSettingsFile(), m_http->getUpdateDocumentMaster()))
    , m_presetManager(
          new PresetManager(m_http->getUpdateDocumentMaster(), false, *m_options, *m_settings, m_audioEngineProxy))
    , m_hwui(new HWUI(*m_settings))
    , m_undoScope(new UndoScope(m_http->getUpdateDocumentMaster()))
    , m_playcontrollerProxy(new PlaycontrollerProxy())
    , m_audioEngineProxy(new AudioEngineProxy(*m_presetManager, *m_settings, *m_playcontrollerProxy))
    , m_voiceGroupManager(std::make_unique<VoiceGroupAndLoadToPartManager>(*m_presetManager->getEditBuffer()))
    , m_watchDog(new WatchDog)
    , m_aggroWatchDog(new WatchDog)
    , m_deviceInformation(new DeviceInformation(m_http->getUpdateDocumentMaster()))
    , m_clipboard(new Clipboard(m_http->getUpdateDocumentMaster()))
    , m_usbChangeListener(std::make_unique<USBChangeListener>())
    , m_webUISupport(std::make_unique<WebUISupport>(m_http->getUpdateDocumentMaster()))
    , m_actionManagers(m_http->getUpdateDocumentMaster(), *m_presetManager, *m_audioEngineProxy, *m_hwui, *m_settings, *m_voiceGroupManager)
    , m_heartbeatState(false)
    , m_isQuit(false)
{
#ifdef _PROFILING
  Profiler::get().enable(true);
#endif

  m_settings->init();
  m_hwui->init();
  m_http->init();
  m_presetManager->init(m_audioEngineProxy.get(), *m_settings, [this](auto str) { m_hwui->addSplashStatus(str); });
  m_hwui->getBaseUnit().getPlayPanel().getSOLED().resetSplash();
  m_voiceGroupManager->init();

  auto focusAndMode = m_settings->getSetting<FocusAndModeSetting>();
  SettingsUseCases useCases(*m_settings);
  useCases.thawFocusAndMode();
  useCases.setFocusAndMode(focusAndMode->getState());

  runWatchDog();

  getMainContext()->signal_timeout().connect(sigc::mem_fun(this, &Application::heartbeat), 500);

  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);

  ::signal(SIGQUIT, quitApp);
  ::signal(SIGTERM, quitApp);
  ::signal(SIGINT, quitApp);

  g_main_context_pop_thread_default(m_theMainContext->gobj());
}

Application::~Application()
{
  stopWatchDog();
  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);

  SpawnAsyncCommandLine::clear();

  m_watchDog.reset();
  m_aggroWatchDog.reset();

  m_hwui->deInit();
  m_hwui.reset();
  m_undoScope.reset();
  m_presetManager.reset();

  nltools::msg::flush(nltools::msg::EndPoint::PanelLed, 1s);
  nltools::msg::flush(nltools::msg::EndPoint::RibbonLed, 1s);
  nltools::msg::flush(nltools::msg::EndPoint::Oled, 1s);

  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);

#ifdef _PROFILING
  Profiler::get().print();
#endif
  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);

  nltools::msg::deInit();

  theApp = nullptr;
}

Application &Application::get()
{
  return *theApp;
}

bool Application::exists()
{
  return theApp != nullptr;
}

void Application::run()
{
  while(!m_isQuit)
  {
    DebugLevel::warning(__PRETTY_FUNCTION__);
    m_theMainLoop->run();
    DebugLevel::warning(__PRETTY_FUNCTION__);
  }
}

void Application::quit()
{
  DebugLevel::warning(__PRETTY_FUNCTION__);

  if(!m_isQuit)
  {
    m_isQuit = true;
    m_theMainLoop->quit();
  }
  else
  {
    DebugLevel::warning("Application already quitting - terminating ungracefully!");
    std::terminate();
  }

  DebugLevel::warning(__PRETTY_FUNCTION__);
}

bool Application::isQuit() const
{
  return m_isQuit;
}

Glib::RefPtr<Glib::MainContext> Application::getMainContext()
{
  return m_theMainLoop->get_context();
}

void Application::runWatchDog()
{
  if(m_watchDog)
    m_watchDog->run(std::chrono::seconds(2));

  if(m_aggroWatchDog)
  {
    m_aggroWatchDog->run(std::chrono::milliseconds(250),
                         [=](int numWarning, int inactiveFoMS)
                         {
                           DebugLevel::warning("Aggro WatchDog was inactive for ", inactiveFoMS, "ms. Warning #",
                                               numWarning);

#ifdef _PROFILING
                           Profiler::get().printAllCallstacks();
#endif

                           if(auto h = getHWUI())
                           {
                             if(getSettings()->getSetting<BlockingMainThreadIndication>()->get())
                             {
                               h->indicateBlockingMainThread();
                             }
                           }
                         });
  }
}

void Application::stopWatchDog()
{
  if(m_watchDog)
    m_watchDog->stop();

  if(m_aggroWatchDog)
    m_aggroWatchDog->stop();
}

PresetManager *Application::getPresetManager() const
{
  return m_presetManager.get();
}

HTTPServer *Application::getHTTPServer()
{
  return m_http.get();
}

Clipboard *Application::getClipboard()
{
  return m_clipboard.get();
}

WebUISupport *Application::getWebUISupport()
{
  return m_webUISupport.get();
}

const Options *Application::getOptions() const
{
  return m_options.get();
}

PlaycontrollerProxy *Application::getPlaycontrollerProxy() const
{
  return m_playcontrollerProxy.get();
}

AudioEngineProxy *Application::getAudioEngineProxy() const
{
  return m_audioEngineProxy.get();
}

Settings *Application::getSettings()
{
  return m_settings.get();
}

HWUI *Application::getHWUI()
{
  return m_hwui.get();
}

const HWUI *Application::getHWUI() const
{
  return m_hwui.get();
}

UndoScope *Application::getUndoScope()
{
  return m_undoScope.get();
}

DeviceInformation *Application::getDeviceInformation()
{
  return m_deviceInformation.get();
}

bool Application::heartbeat()
{
  m_heartbeatState = !m_heartbeatState;

  const char *toSend = m_heartbeatState ? "1" : "0";

  if(int out = open("/sys/class/leds/playground_status/brightness", O_WRONLY))
  {
    nltools::ignore(write(out, toSend, 1));
    fsync(out);
    close(out);
  }

  if(m_heartbeatState)
  {
    int transactionsSinceLastCall = UNDO::Transaction::getAndResetNumTransactions();

    if(transactionsSinceLastCall)
      DebugLevel::info("Undo transactions are created at speed of:", transactionsSinceLastCall, "transactions/s");
  }

  return true;
}

ActionManagers *Application::getActionManagers()
{
  return &m_actionManagers;
}

VoiceGroupAndLoadToPartManager *Application::getVGManager()
{
  return m_voiceGroupManager.get();
}
