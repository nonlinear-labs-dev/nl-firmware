#include "third-party/include/catch.hpp"
#include "testing/TestHelper.h"
#include <presets/Bank.h>

TEST_CASE("Preset Manager Init") {
  auto pm = TestHelper::getPresetManager();
  REQUIRE(pm != nullptr);
}

TEST_CASE("Create Bank") {
  auto pm = TestHelper::getPresetManager();
  {
    auto scope = TestHelper::createTestScope();
    pm->clear(scope->getTransaction());
    REQUIRE(pm->getNumBanks() == 0);
  }

  SECTION("New Bank")
  {
    auto scope = TestHelper::createTestScope();
    auto newBank = pm->addBank(scope->getTransaction());
    REQUIRE(newBank != nullptr);
    REQUIRE(pm->findBank(newBank->getUuid()) == newBank);
    REQUIRE(pm->getNumBanks() == 1);
  }
}