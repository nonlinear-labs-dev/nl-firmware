#include <testing/TestHelper.h>
#include <presets/Bank.h>
#include "CompileTimeOptions.h"
#include <iostream>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Sort Bank Numbers deletes Banks!", "[2905]")
{
  std::cout << __LINE__ << std::endl;
  auto pm = TestHelper::getPresetManager();
  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
  }

  std::cout << __LINE__ << std::endl;
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);

  {
    FileInStream stream(
        getSourceDir() + "/projects/epc/playground/test-resources/issue-2905-nonlinear-c15-banks.xml.tar", true);
    uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
  }

  std::cout << __LINE__ << std::endl;
  WHEN("Banks are Loaded")
  {
    const auto numBanksPreSort = pm->getNumBanks();
    CHECK(numBanksPreSort > 0);

    std::cout << __LINE__ << std::endl;
    auto bankThatWillBeLost = pm->getBankAt(20);
    auto bankThatWillBeLost2 = pm->getBankAt(21);

    CHECK(bankThatWillBeLost->getName(false) == "David Bank 1");
    CHECK(bankThatWillBeLost2->getName(false) == "David Work 2");

    std::cout << __LINE__ << std::endl;
    uc.sortBankNumbers();
    std::cout << __LINE__ << std::endl;
    const auto numBanksAfterSort = pm->getNumBanks();
    CHECK(numBanksAfterSort == numBanksPreSort);
  }
}