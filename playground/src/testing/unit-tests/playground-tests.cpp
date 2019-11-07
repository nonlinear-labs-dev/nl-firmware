#define CATCH_CONFIG_RUNNER
#include <third-party/include/catch.hpp>
#include "Application.h"
#include "testing/TestDriver.h"
#include "device-settings/DebugLevel.h"
#include "playground-helpers.h"

int main(int numArgs, char** argv)
{
  Gio::init();

  Environment::setupLocale();

  ::signal(SIGSEGV, Environment::printStackTrace);
  ::signal(SIGILL, Environment::printStackTrace);
  ::signal(SIGBUS, Environment::printStackTrace);
  ::signal(SIGKILL, Environment::printStackTrace);

  TestDriverBase::doTests(numArgs, argv);
  Application app(numArgs, argv);

  app.stopWatchDog();

  nltools::Log::setLevel(nltools::Log::Silent);

  std::vector<const char*> args;
  args.emplace_back(argv[0]);
  //args.emplace_back("-s");  //this line enables showing of passed tests

  int result = Catch::Session().run(args.size(), args.data());
  return result;
}
