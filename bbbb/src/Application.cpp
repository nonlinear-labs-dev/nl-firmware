#include "Application.h"
#include "io/Bridges.h"
#include <fcntl.h>
#include "Options.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Application *Application::theApp = nullptr;

char *Application::initStatic(Application *app, char *argv)
{
  theApp = app;
  return argv;
}

void quitApp(int sig)
{
  TRACE(sig);
  Application::get().quit();
}

Application::Application(int numArgs, char **argv) :
    m_selfPath(initStatic(this, argv[0])),
    m_options(std::make_unique<Options>(numArgs, argv)),
    m_bridges(std::make_unique<Bridges>()),
    m_theMainLoop(Glib::MainLoop::create()),
    m_isQuit(false)
{
  ::signal(SIGQUIT, quitApp);
  ::signal(SIGTERM, quitApp);
  ::signal(SIGINT, quitApp);
}

Application::~Application()
{
}

Application &Application::get()
{
  return *theApp;
}

Glib::ustring Application::getSelfPath() const
{
  return m_selfPath;
}

void Application::run()
{
  m_theMainLoop->run();
}

void Application::quit()
{
  m_isQuit = true;
  m_theMainLoop->quit();
}

bool Application::isQuit() const
{
  return m_isQuit;
}

Glib::RefPtr<Glib::MainContext> Application::getMainContext()
{
  return m_theMainLoop->get_context();
}

Options *Application::getOptions()
{
  return m_options.get();
}
