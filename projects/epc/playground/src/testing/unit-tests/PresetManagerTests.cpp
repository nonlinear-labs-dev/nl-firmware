#include <catch.hpp>
#include "testing/TestHelper.h"
#include "mock/MockPresetStorage.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <use-cases/PresetUseCases.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Preset Manager Init")
{
  auto pm = TestHelper::getPresetManager();
  REQUIRE(pm != nullptr);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Create Bank")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  useCase.clear();

  SECTION("New Bank")
  {
    auto newBank = useCase.createBankAndStoreEditBuffer();
    REQUIRE(newBank != nullptr);
    REQUIRE(pm->findBank(newBank->getUuid()) == newBank);
    REQUIRE(pm->getNumBanks() == 1);
  }
}

namespace EditBufferHelper
{
  template <SoundType tSoundType> void convertEditBufferToDual()
  {
    auto eb = TestHelper::getEditBuffer();
    EditBufferUseCases useCase(*eb);
    useCase.convertToDual(tSoundType, VoiceGroup::I);
    REQUIRE(eb->getType() == tSoundType);
  }

  void convertEditBufferToSingle()
  {
    auto eb = TestHelper::getEditBuffer();
    EditBufferUseCases useCases(*eb);
    useCases.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getType() == SoundType::Single);
  }

  void overwritePresetWithEditBuffer(Preset* p)
  {
    auto eb = TestHelper::getEditBuffer();
    auto ebType = eb->getType();

    PresetUseCases useCase(*p, *TestHelper::getSettings());
    useCase.overwriteWithEditBuffer(*eb);

    REQUIRE(p->getType() == ebType);
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Overwrite Presets")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  useCase.clear();
  MockPresetStorage presets;

  SECTION("Overwrite Single with Single")
  {
    EditBufferHelper::convertEditBufferToSingle();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getSinglePreset());
  }

  SECTION("Overwrite Single with Split")
  {
    EditBufferHelper::convertEditBufferToDual<SoundType::Split>();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getSinglePreset());
  }

  SECTION("Overwrite Single with Layer")
  {
    EditBufferHelper::convertEditBufferToDual<SoundType::Layer>();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getSinglePreset());
  }

  SECTION("Overwrite Layer with Single")
  {
    EditBufferHelper::convertEditBufferToSingle();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getLayerPreset());
  }

  SECTION("Overwrite Layer with Split")
  {
    EditBufferHelper::convertEditBufferToDual<SoundType::Split>();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getLayerPreset());
  }

  SECTION("Overwrite Layer with Layer")
  {
    EditBufferHelper::convertEditBufferToDual<SoundType::Layer>();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getLayerPreset());
  }

  SECTION("Overwrite Split with Single")
  {
    EditBufferHelper::convertEditBufferToSingle();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getSplitPreset());
  }

  SECTION("Overwrite Split with Split")
  {
    EditBufferHelper::convertEditBufferToDual<SoundType::Split>();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getSplitPreset());
  }

  SECTION("Overwrite Split with Layer")
  {
    EditBufferHelper::convertEditBufferToDual<SoundType::Layer>();
    EditBufferHelper::overwritePresetWithEditBuffer(presets.getSplitPreset());
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Midi Selection Sends Signals Appropriately")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  useCase.clear();

  CHECK(pm->getNumBanks() == 0);

  MockPresetStorage presets;
  MockPresetStorage presets2;

  CHECK(pm->getNumBanks() == 2);

  WHEN("Midi Bank Selected")
  {
    const auto oldMidiUuid = pm->getMidiSelectedBank();
    Uuid receivedMidiUuid;

    auto c = pm->onMidiBankSelectionHappened([&](const Uuid& id) { receivedMidiUuid = id; });

    useCase.selectMidiBank(presets.getBank());

    CHECK(presets.getBank()->getUuid() == receivedMidiUuid);

    THEN("UNDO")
    {
      pm->getUndoScope().undo();
      CHECK(oldMidiUuid == receivedMidiUuid);

      THEN("REDO")
      {
        pm->getUndoScope().redo();
        CHECK(presets.getBank()->getUuid() == receivedMidiUuid);
        CHECK(pm->findMidiSelectedBank() != nullptr);
      }
    }
    c.disconnect();
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Delete Current Midi Bank resets attribute")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCases(*pm, *settings);
  useCases.clear();

  //Create bank to use
  useCases.createBankAndStoreEditBuffer();
  auto b = pm->getSelectedBank();
  CHECK(pm->getNumBanks() == 1);
  CHECK(b != nullptr);

  //select bank as midi target
  useCases.selectMidiBank(b);
  CHECK(pm->getMidiSelectedBank() == b->getUuid());

  WHEN("Bank Deleted")
  {
    useCases.deleteBank(b);
    CHECK(pm->getNumBanks() == 0);
    CHECK(pm->getMidiSelectedBank() == Uuid::none());
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "When Midi Bank is Selected new and old midi selection banks get notified via onChange")
{
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCases(*pm, *settings);
  useCases.clear();

  useCases.createBankAndStoreEditBuffer();
  auto b1 = pm->getSelectedBank();

  useCases.createBankAndStoreEditBuffer();
  auto b2 = pm->getSelectedBank();

  int changesB1 = 0;
  int changesB2 = 0;
  auto c1 = b1->onBankChanged([&]() { changesB1++; });
  auto c2 = b2->onBankChanged([&]() { changesB2++; });

  WHEN("Midi Bank 1 Selected")
  {
    const auto b1ChangesBeforeSelection = changesB1;
    useCases.selectMidiBank(b1);
    CHECK(changesB1 > b1ChangesBeforeSelection);
    const auto b1ChangesAfterSelection = changesB1;

    THEN("Midi Bank 2 Selected")
    {
      const auto b2ChangesBeforeSelection = changesB2;
      useCases.selectMidiBank(b2);
      CHECK(changesB2 > b2ChangesBeforeSelection);
      CHECK(changesB1 > b1ChangesAfterSelection);
    }
  }

  c1.disconnect();
  c2.disconnect();
}