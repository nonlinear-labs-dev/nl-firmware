#pragma once

#include "bbbb.h"

class Options;
class Bridges;

class Application
{
  public:
    Application (int numArgs, char **argv);
    virtual ~Application ();

    Glib::ustring getSelfPath() const;

    static Application &get();

    void run ();
    void quit();
    bool isQuit () const;

    Glib::RefPtr<Glib::MainContext> getMainContext();
    Options *getOptions();

  private:
    static char *initStatic (Application *app, char *argv);

    static Application *theApp;
    Glib::ustring m_selfPath;
    std::unique_ptr<Options> m_options;
    std::unique_ptr<Bridges> m_bridges;
    Glib::RefPtr<Glib::MainLoop> m_theMainLoop;
    bool m_isQuit;
};
