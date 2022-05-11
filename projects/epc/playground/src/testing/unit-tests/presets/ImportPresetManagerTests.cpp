#include "testing/TestHelper.h"
#include "presets/Preset.h"
#include "presets/PresetManager.h"
#include <xml/MemoryInStream.h>
#include <xml/MemoryOutStream.h>
#include <serialization/PresetManagerSerializer.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "CompileTimeOptions.h"
#include <tools/StringTools.h>
#include <nltools/logging/Log.h>
#include <xml/ZippedMemoryOutStream.h>
#include <xml/VersionAttribute.h>
#include <libundo/undo/TrashTransaction.h>
#include <iostream>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Import PresetManager consumes memory as expected")
{
  std::cout << __LINE__ << std::endl;
  auto pm = TestHelper::getPresetManager();
  {
    std::cout << __LINE__ << std::endl;
    auto trash = UNDO::Scope::startTrashTransaction();
    std::cout << __LINE__ << std::endl;
    pm->clear(trash->getTransaction());
    std::cout << __LINE__ << std::endl;
  }

  std::cout << __LINE__ << std::endl;
  //auto memUsage = nltools::Log::MemUsageLogger(nltools::Log::Debug, __PRETTY_FUNCTION__);
  std::cout << __LINE__ << std::endl;
  {
    auto settings = TestHelper::getSettings();
    std::cout << __LINE__ << std::endl;
    PresetManagerUseCases uc(*pm, *settings);
    std::cout << __LINE__ << std::endl;
    FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/one_bank.xml.tar", true);
    std::cout << __LINE__ << std::endl;
    uc.importBackupFile(stream, {}, TestHelper::getAudioEngineProxy());
    std::cout << __LINE__ << std::endl;
  }

  std::cout << __LINE__ << std::endl;
  //CHECK(memUsage.getUsage() < 9 * 1024 * 1024);
  std::cout << __LINE__ << std::endl;
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Import PresetManager overwrites banks")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);
  uc.clear();

  auto memUsage = nltools::Log::MemUsageLogger(nltools::Log::Debug, __PRETTY_FUNCTION__);
  FileInStream oneStream(getSourceDir() + "/projects/epc/playground/test-resources/one_bank.xml.tar", true);
  uc.importBackupFile(oneStream, {}, TestHelper::getAudioEngineProxy());
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "Ulis Bank 07");

  FileInStream otherStream(getSourceDir() + "/projects/epc/playground/test-resources/other_bank.xml.tar", true);
  uc.importBackupFile(otherStream, {}, TestHelper::getAudioEngineProxy());
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "NL Weird 06");

  CHECK(memUsage.getUsage() < 4.5 * 1024 * 1024);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Import PresetManager is undoable")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);
  uc.clear();

  auto memUsage = nltools::Log::MemUsageLogger(nltools::Log::Debug, __PRETTY_FUNCTION__);
  FileInStream oneStream(getSourceDir() + "/projects/epc/playground/test-resources/one_bank.xml.tar", true);
  uc.importBackupFile(oneStream, {}, TestHelper::getAudioEngineProxy());
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "Ulis Bank 07");

  FileInStream otherStream(getSourceDir() + "/projects/epc/playground/test-resources/other_bank.xml.tar", true);
  uc.importBackupFile(otherStream, {}, TestHelper::getAudioEngineProxy());
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "NL Weird 06");

  pm->getUndoScope().undo();

  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "Ulis Bank 07");

  pm->getUndoScope().redo();

  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "NL Weird 06");

  CHECK(memUsage.getUsage() < 4.5 * 1024 * 1024);
}
