#include "Application.h"
#include "device-settings/DebugLevel.h"
#include "Options.h"
#include "playground-helpers.h"
#include <giomm.h>
#include <nltools/messaging/Messaging.h>


{
  std::vector<void*> stack;
  stack.reserve(maxFrames);
  auto numFrames = backtrace(stack.data(), maxFrames);
  if(numFrames == maxFrames)
  {
    nltools::Log::error("You could be missing some frames from your backtrace. Try to increase your maxFrames");
  }

  auto symbols = backtrace_symbols(stack.data(), numFrames); //symbols is mallocced here

  for(auto i = 0; i < numFrames; i++)
  {
    nltools::Log::printWithLevel(level, i, symbols[i]);
  }

  free(symbols);
}

int main(int numArgs, char **argv)
{
  Glib::init();
  Gio::init();

  Environment::setupLocale();

  ::signal(SIGSEGV, Environment::printStackTrace);
  ::signal(SIGILL, Environment::printStackTrace);
  ::signal(SIGBUS, Environment::printStackTrace);
  ::signal(SIGKILL, Environment::printStackTrace);

#ifdef _PROFILING
  Profiler::get().enable(true);
#endif

  {
    Application app(numArgs, argv);
    Application::get().run();
    DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);
  }

  DebugLevel::warning(__PRETTY_FUNCTION__, __LINE__);
  return EXIT_SUCCESS;
}
