#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include "xml/VersionAttribute.h"
#include "tools/ExceptionTools.h"
#include <proxies/hwui/HWUI.h>

TEST_CASE("File in Stream")
{
  CHECK(StringTools::hasEnding("/tmp/test.xml.tar.gz", ".gz") == true);
  CHECK(StringTools::hasEnding("/tmp/test.xml.tar.gez", ".gz") == false);
  CHECK(StringTools::hasEnding("gz./tmp/test.xml.tar.gz", ".gz") == true);
  CHECK(StringTools::hasEnding("/tmp/gz.test.xml.gz.tar.gz.gez", ".gz") == false);
  CHECK(StringTools::hasEnding("", ".gz") == false);

  CHECK(StringTools::hasEnding("/tmp/test.xml.tar.zip", ".zip") == true);
  CHECK(StringTools::hasEnding("/tmp/test.xml.tar.zp", ".zip") == false);
  CHECK(StringTools::hasEnding(".zip./tmp/test.xml.tar.zip", ".zip") == true);
  CHECK(StringTools::hasEnding("/tmp/zip.test.xml.zip.tar.zip.zeip", ".zip") == false);
  CHECK(StringTools::hasEnding("", ".zip") == false);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "3343 - Can't Load Backup")
{
  const auto backupFileNotBroken
      = getSourceDir() + "/projects/epc/playground/test-resources/2022-10-04-15-30-nonlinear-c15-banks.xml.tar.gz";

  const auto backupFileVersion99999
      = getSourceDir() + "/projects/epc/playground/test-resources/version-999999.xml.tar.gz";

  const auto fakeZipFile = getSourceDir() + "/projects/epc/playground/test-resources/not-a-real-zip.zip";

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

  WHEN("invalid file is loaded (try zip)")
  {
    FileInStream stream(fakeZipFile, true);
    auto ret = uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::InvalidFile);
  }

  WHEN("invalid file is loaded")
  {
    FileInStream stream(fakeZipFile, false);
    auto ret = uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::InvalidFile);
  }

  WHEN("load backup with older or same version, size of data is same than on disk")
  {
    constexpr auto sizeOnDisk = 149510100;
    FileInStream stream2(backupFileNotBroken, true);
    auto data = stream2.readAll();
    CHECK(data.size() == sizeOnDisk);
  }

  WHEN("load backup with older or same version")
  {
    FileInStream stream(backupFileNotBroken, true);
    auto ret = uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    CHECK(ret == PresetManagerUseCases::ImportExitCode::OK);
  }

  Application::get().runWatchDog();
}