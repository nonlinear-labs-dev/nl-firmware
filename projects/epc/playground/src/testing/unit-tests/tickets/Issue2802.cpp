#include <testing/TestHelper.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <use-cases/PresetUseCases.h>

/*
 * Preset über Soundscreen in Single/Split oder Layer konvertieren (Presetname ist automatisch "Converted").
 * Das konvertierte Preset (editieren und) mit Append speichern und dabei einen neuen Namen eingeben.
 * Das Preset noch einmal editieren und mit Overwrite auf demselben Platz speichern, der Name ist leider wieder "Converted"
 */

// "Converted" is never the name of the Editbuffer we set.

TEST_CASE("Ticket 2802")
{
  auto& eb = *TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(eb);
  auto& pm = *TestHelper::getPresetManager();
  auto& settings = *TestHelper::getSettings();
  PresetManagerUseCases pmUseCases(pm, settings);
  auto tmpBank = pmUseCases.createBankAndStoreEditBuffer();
  auto tmpPreset = tmpBank->getPresetAt(0);
  PresetUseCases presetUseCases(*tmpPreset, settings);

  WHEN("Single Editbuffer converted to Layer")
  {
    TestHelper::initSingleEditBuffer();
    ebUseCases.convertToLayer(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");
  }

  WHEN("Single Editbuffer converted to Split")
  {
    TestHelper::initSingleEditBuffer();
    ebUseCases.convertToSplit(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");
  }

  WHEN("Layer Editbuffer converted to Single")
  {
    TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
    ebUseCases.convertToSingle(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");
  }

  WHEN("Layer Editbuffer converted to Split")
  {
    TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
    ebUseCases.convertToSingle(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");
  }

  WHEN("Split Editbuffer converted to Layer")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    ebUseCases.convertToLayer(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");
  }

  WHEN("Split Editbuffer converted to Single")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    ebUseCases.convertToSingle(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");
  }
}