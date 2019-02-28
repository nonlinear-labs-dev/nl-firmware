#include "Options.h"
#include "synth/SimpleSynth.h"
#include "synth/C15Synth.h"
#include <glibmm.h>
#include <iostream>

static Glib::RefPtr<Glib::MainLoop> theMainLoop;
static std::unique_ptr<Options> theOptions;

static void quit(int)
{
  if(theMainLoop)
    theMainLoop->quit();
}

void connectSignals()
{
  signal(SIGINT, quit);
  signal(SIGQUIT, quit);
  signal(SIGTERM, quit);
}

const Options *getOptions()
{
  return theOptions.get();
}

void runMainLoop()
{
  theMainLoop = Glib::MainLoop::create();
  theMainLoop->run();
}

int main(int args, char *argv[])
{
  Glib::init();

  connectSignals();

  theOptions = std::make_unique<Options>(args, argv);

  //auto synth = std::make_unique<SimpleSynth>();
  auto synth = std::make_unique<C15Synth>();
  synth->start();
  runMainLoop();
  synth->stop();

  return EXIT_SUCCESS;
}
