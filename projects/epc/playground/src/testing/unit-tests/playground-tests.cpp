#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "Application.h"
#include "device-settings/DebugLevel.h"
#include "playground-helpers.h"
#include <giomm.h>
#include <Options.h>

int main(int numArgs, char **argv)
{
  nltools::Log::setLevel(nltools::Log::Silent);

  Gio::init();

  Environment::setupLocale();

  Options::s_acceptanceTests = true;

  return Catch::Session().run(numArgs, argv);
}
