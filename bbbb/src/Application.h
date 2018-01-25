#pragma once

#include "bbbb.h"
#include <gtkmm-3.0/gtkmm.h>

class Options;
class Bridges;
class WebSocketServer;

class Application
{
  public:
    Application (int numArgs, char **argv);
    virtual ~Application ();

    Glib::ustring getSelfPath() const;

    static Application &get();

    void run ();
    void runWithWindow ();

    Options *getOptions();
    WebSocketServer *getWebsocketServer();

  private:
    static void activate (GtkApplication* app, Application *pThis);
    static char *initStatic (Application *app, char *argv);

    static Application *theApp;
    Glib::ustring m_selfPath;
    std::unique_ptr<Options> m_options;
    std::unique_ptr<WebSocketServer> m_websocketServer;
    std::unique_ptr<Bridges> m_bridges;
};
