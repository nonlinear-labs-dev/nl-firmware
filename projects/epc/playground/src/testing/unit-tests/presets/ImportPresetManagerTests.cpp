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

TEST_CASE("Import PresetManager consumes memory as expected")
{
  auto pm = TestHelper::getPresetManager();
  auto trash = UNDO::Scope::startTrashTransaction();
  pm->clear(trash->getTransaction());

  auto memUsage = nltools::Log::MemUsageLogger(nltools::Log::Debug, __PRETTY_FUNCTION__);
  {
    PresetManagerUseCases uc(pm);
    FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/one_bank.xml.tar", true);
    uc.importBackupFile(stream, {});
  }

  CHECK(memUsage.getUsage() < 8 * 1024 * 1024);
}

TEST_CASE("Import PresetManager overwrites banks")
{
  auto pm = TestHelper::getPresetManager();
  auto trash = UNDO::Scope::startTrashTransaction();
  pm->clear(trash->getTransaction());

  PresetManagerUseCases uc(pm);
  auto memUsage = nltools::Log::MemUsageLogger(nltools::Log::Debug, __PRETTY_FUNCTION__);
  FileInStream oneStream(getSourceDir() + "/projects/epc/playground/test-resources/one_bank.xml.tar", true);
  uc.importBackupFile(oneStream, {});
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "Ulis Bank 07");

  FileInStream otherStream(getSourceDir() + "/projects/epc/playground/test-resources/other_bank.xml.tar", true);
  uc.importBackupFile(otherStream, {});
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "NL Weird 06");

  CHECK(memUsage.getUsage() < 4 * 1024 * 1024);
}

TEST_CASE("Import PresetManager is undoable")
{
  auto pm = TestHelper::getPresetManager();
  auto trash = UNDO::Scope::startTrashTransaction();
  pm->clear(trash->getTransaction());

  PresetManagerUseCases uc(pm);
  auto memUsage = nltools::Log::MemUsageLogger(nltools::Log::Debug, __PRETTY_FUNCTION__);
  FileInStream oneStream(getSourceDir() + "/projects/epc/playground/test-resources/one_bank.xml.tar", true);
  uc.importBackupFile(oneStream, {});
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "Ulis Bank 07");

  FileInStream otherStream(getSourceDir() + "/projects/epc/playground/test-resources/other_bank.xml.tar", true);
  uc.importBackupFile(otherStream, {});
  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "NL Weird 06");

  pm->getUndoScope().undo();

  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "Ulis Bank 07");

  pm->getUndoScope().redo();

  REQUIRE(pm->getBanks().size() == 1);
  REQUIRE(pm->getBanks().front()->getName(false) == "NL Weird 06");

  CHECK(memUsage.getUsage() < 4 * 1024 * 1024);
}
