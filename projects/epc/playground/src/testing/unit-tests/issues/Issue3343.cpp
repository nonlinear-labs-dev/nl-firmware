#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include "xml/VersionAttribute.h"
#include "tools/ExceptionTools.h"
#include <proxies/hwui/HWUI.h>

TEST_CASE("File in Stream")
{
  CHECK(FileInStream::doesPathEndWithGZ("/tmp/test.xml.tar.gz") == true);
  CHECK(FileInStream::doesPathEndWithGZ("/tmp/test.xml.tar.gez") == false);
  CHECK(FileInStream::doesPathEndWithGZ("gz./tmp/test.xml.tar.gz") == true);
  CHECK(FileInStream::doesPathEndWithGZ("/tmp/gz.test.xml.gz.tar.gz.gez") == false);
  CHECK(FileInStream::doesPathEndWithGZ("") == false);

  CHECK(FileInStream::doesPathEndWithZip("/tmp/test.xml.tar.zip") == true);
  CHECK(FileInStream::doesPathEndWithZip("/tmp/test.xml.tar.zp") == false);
  CHECK(FileInStream::doesPathEndWithZip(".zip./tmp/test.xml.tar.zip") == true);
  CHECK(FileInStream::doesPathEndWithZip("/tmp/zip.test.xml.zip.tar.zip.zeip") == false);
  CHECK(FileInStream::doesPathEndWithZip("") == false);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "3343 - Can't Load Backup")
{
  const auto backupFileNotBroken
      = getSourceDir() + "/projects/epc/playground/test-resources/2022-10-04-15-30-nonlinear-c15-banks.xml.tar.gz";

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