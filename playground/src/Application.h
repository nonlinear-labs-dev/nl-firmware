#pragma once

#include "playground.h"

class DeviceInformation;
class WatchDog;
class HTTPServer;
class PresetManager;
class LPCProxy;
class HWUI;
class Options;
class HWTests;
class UndoScope;
class Settings;
class EmbeddedPC;
class Clipboard;
class WebSocketSession;

class Application
{
 public:
  Application(int numArgs, char **argv);
  virtual ~Application();

  Glib::ustring getSelfPath() const;
  Glib::ustring getResourcePath() const;

  static Application &get();

  void run();
  RefPtr<MainContext> getMainContext();

  PresetManager *getPresetManager() const;
  HTTPServer *getHTTPServer();
  Options *getOptions();
  LPCProxy *getLPCProxy() const;
  HWTests *getHWTests();
  HWUI *getHWUI();
  const HWUI *getHWUI() const;
  UndoScope *getUndoScope();
  Settings *getSettings();
  DeviceInformation *getDeviceInformation();
  Clipboard *getClipboard();

  WebSocketSession *getWebSocketSession();

  void quit();
  bool isQuit() const;

  void runWatchDog();
  void stopWatchDog();

 private:
  bool heartbeat();
  static char *initStatic(Application *app, char *argv);

  static Application *theApp;
  Glib::ustring m_selfPath;
  RefPtr<MainLoop> m_theMainLoop;

  std::unique_ptr<Options> m_options;
  std::unique_ptr<WebSocketSession> m_websocketSession;
  std::unique_ptr<HTTPServer> m_http;
  std::unique_ptr<Settings> m_settings;
  std::unique_ptr<UndoScope> m_undoScope;
  std::unique_ptr<PresetManager> m_presetManager;
  std::unique_ptr<LPCProxy> m_lpcProxy;
  std::unique_ptr<HWUI> m_hwui;
  std::unique_ptr<HWTests> m_hwtests;

  std::unique_ptr<WatchDog> m_watchDog;
  std::unique_ptr<WatchDog> m_aggroWatchDog;
  std::unique_ptr<DeviceInformation> m_deviceInformation;
  std::unique_ptr<Clipboard> m_clipboard;

  bool m_heartbeatState;
  bool m_isQuit;

  friend class AutoSaveInterval;
};
