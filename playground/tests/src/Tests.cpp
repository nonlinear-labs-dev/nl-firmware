#include "Tests.h"
#include "Options.h"
#include "LoadScriptJob.h"
#include "ParseScriptJob.h"
#include "ExecuteScriptJob.h"

#include <libxml/xpath.h>

static Tests *theTests = nullptr;

void quitApp(int sig)
{
  theTests->quit();
}

Tests::Tests(int numArgs, char **argv)
    : super(nullptr)
    , m_options(new Options(numArgs, argv))
    , m_theMainLoop(MainLoop::create())
{
  theTests = this;

  ::signal(SIGQUIT, quitApp);
  ::signal(SIGTERM, quitApp);
  ::signal(SIGINT, quitApp);
}

Tests::~Tests()
{
}

Job *Tests::getRoot()
{
  return this;
}

shared_ptr<Options> Tests::getOptions()
{
  return m_options;
}

void Tests::start()
{
  MainContext::get_default()->signal_idle().connect_once(mem_fun(this, &Tests::startTest));
  m_theMainLoop->run();
}

void Tests::startTest()
{
  add<LoadScriptJob>();
  add<ParseScriptJob>();
  add<ExecuteScriptJob>();

  run();
}

void Tests::implRun()
{
  quit();
}

void Tests::onChildFinished(Job *child, bool success)
{
  if(success)
    super::onChildFinished(child, true);
  else
    quit();
}

void Tests::quit()
{
  m_theMainLoop->quit();
}

int main(int numArgs, char **argv)
{
  Gio::init();
  xmlXPathInit();
  xmlInitParser();
  Tests app(numArgs, argv);
  app.start();
  return 1;
}
