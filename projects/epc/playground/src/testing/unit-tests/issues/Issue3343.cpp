#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include "xml/VersionAttribute.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "3343 - Can't Load Backup")
{
  const auto backupFileVersion14Broken
      = getSourceDir() + "/projects/epc/playground/test-resources/2022-10-04-15-30-nonlinear-c15-banks.xml.tar.gz";

  const auto backupFileVersion14NotBroken
      = getSourceDir() + "/projects/epc/playground/test-resources/2022-10-03-10-33-nonlinear-c15-banks.xml.tar.gz";

  const auto backupFileVersion99999
      = getSourceDir() + "/projects/epc/playground/test-resources/version-999999.xml.tar.gz";

  Application::get().stopWatchDog();

  auto pm = TestHelper::getPresetManager();
  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
  }

  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);

  WHEN("load backup newer than version")
  {
    auto version = VersionAttribute::getCurrentFileVersion();
    REQUIRE(version < 999999);
    FileInStream stream(backupFileVersion99999, true);
    auto ret = uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::UnsupportedVersion);
  }

  WHEN("load backup with older or same version, size of data is same than on disk")
  {
    constexpr auto sizeOnDisk = 7679985;
    FileInStream stream2(backupFileVersion14Broken, true);
    auto data = stream2.readAll();
    CHECK(data.size() == sizeOnDisk);
  }

  WHEN("load backup with older or same version")
  {
    FileInStream stream(backupFileVersion14Broken, true);
    auto ret = uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::OK);
  }

  Application::get().runWatchDog();
}