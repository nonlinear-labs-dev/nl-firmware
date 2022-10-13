#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "3343 - Can't Load Backup")
{
  const auto backupFileVersion14Broken
      = getSourceDir() + "/projects/epc/playground/test-resources/2022-10-04-15-30-nonlinear-c15-banks.xml.tar.gz";

  const auto backupFileVersion14NotBroken
      = getSourceDir() + "/projects/epc/playground/test-resources/2022-10-03-10-33-nonlinear-c15-banks.xml.tar.gz";

  Application::get().stopWatchDog();

  auto pm = TestHelper::getPresetManager();
  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
  }

  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);

  {
    FileInStream stream2(backupFileVersion14NotBroken, true);
    auto ret = uc.importBackupFile(stream2, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::OK);
  }

  {
    FileInStream stream2(backupFileVersion14Broken, true);
    auto data = stream2.readAll();
    CHECK(data.size() == 7679985);  //Like on Disk
  }

  {
    FileInStream stream(backupFileVersion14Broken, true);
    auto ret = uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::OK);
  }

  Application::get().runWatchDog();
}