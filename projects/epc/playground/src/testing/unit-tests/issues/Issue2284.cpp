#include <testing/TestHelper.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <parameter_declarations.h>
#include <use-cases/PresetUseCases.h>
#include <proxies/hwui/panel-unit/boled/setup/ExportBackupEditor.h>
#include "xml/MemoryOutStream.h"
#include "xml/MemoryInStream.h"
#include "xml/FileOutStream.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Export Backup with MIDI Bank exports MIDI UUID")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases pmUseCases(*pm, *settings);
  auto bank = pmUseCases.createBankAndStoreEditBuffer();
  pmUseCases.selectMidiBank(bank);

  CHECK(bank->isMidiSelectedBank());

  const auto fileName = "/tmp/issue-2284.tar";

  std::filesystem::remove(fileName);

  {
    FileOutStream outStream(fileName, false);
    ExportBackupEditor::writeBackupToStream(outStream);
  }

  auto oldNumBanks = pm->getNumBanks();
  auto uuidOfBank = bank->getUuid();

  pmUseCases.clear();


  CHECK(pm->findBank(uuidOfBank) == nullptr);

  {
    FileInStream inStream(fileName, false);
    pmUseCases.importBackupFile(inStream, {}, TestHelper::getAudioEngineProxy());
  }

  REQUIRE(pm->getNumBanks() == oldNumBanks);
  REQUIRE(pm->findBank(uuidOfBank) != nullptr);
  bank = pm->findBank(uuidOfBank);
  CHECK(bank->isMidiSelectedBank());
}