#include <testing/TestHelper.h>
#include <presets/Bank.h>
#include "CompileTimeOptions.h"
#include <iostream>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Sort Bank Numbers deletes Banks!", "[2905]")
{
  Application::get().stopWatchDog();

  auto pm = TestHelper::getPresetManager();
  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
  }

  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);

  {
    FileInStream stream(
        getSourceDir() + "/projects/epc/playground/test-resources/issue-2905-nonlinear-c15-banks.xml.tar", true);
    uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
  }

  WHEN("Banks are Loaded")
  {
    const auto numBanksPreSort = pm->getNumBanks();
    CHECK(numBanksPreSort > 0);

    auto bankThatWillBeLost = pm->getBankAt(20);
    auto bankThatWillBeLost2 = pm->getBankAt(21);

    CHECK(bankThatWillBeLost->getName(false) == "David Bank 1");
    CHECK(bankThatWillBeLost2->getName(false) == "David Work 2");

    uc.sortBankNumbers();
    const auto numBanksAfterSort = pm->getNumBanks();
    CHECK(numBanksAfterSort == numBanksPreSort);
  }
}