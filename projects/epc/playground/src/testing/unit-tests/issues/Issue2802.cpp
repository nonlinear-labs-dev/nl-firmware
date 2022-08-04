#include "testing/TestHelper.h"
#include "presets/Bank.h"
#include "presets/Preset.h"
#include "use-cases/PresetUseCases.h"
#include <parameter_declarations.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Ticket 2802")
{
  auto& eb = *TestHelper::getEditBuffer();
  auto& pm = *TestHelper::getPresetManager();
  auto& settings = *TestHelper::getSettings();

  EditBufferUseCases ebUseCases(eb);
  PresetManagerUseCases pmUseCases(pm, settings);
  auto tmpBank = pmUseCases.createBankAndStoreEditBuffer();
  auto tmpPreset = tmpBank->getPresetAt(0);

  BankUseCases bankUseCases(tmpBank, settings);
  PresetUseCases presetUseCases(*tmpPreset, settings);

  ParameterUseCases parameterUseCases(eb.findParameterByID(ParameterId{C15::PID::Env_A_Att, VoiceGroup::I}));

  WHEN("Simple Test as described in the Ticket")
  {
    //Convert Preset
    ebUseCases.convertToSingle(VoiceGroup::I);
    //Edit
    parameterUseCases.incDec(1, false, false);
    //Append and Rename
    auto savedPreset = bankUseCases.appendEditBuffer();
    PresetUseCases savedPresetUseCase(*savedPreset, settings);
    savedPresetUseCase.rename("Foo");
    const auto oldName = savedPreset->getName();
    CHECK(eb.getName() == "Foo");
    //Edit
    parameterUseCases.incDec(1, false, false);
    //Overwrite
    savedPresetUseCase.overwriteWithEditBuffer(eb);
    CHECK(savedPreset->getName() == oldName);
  }

  WHEN("Single Editbuffer converted to Layer")
  {
    TestHelper::initSingleEditBuffer();
    ebUseCases.convertToLayer(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");

    WHEN("Renamed without changes to EB")
    {
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }

    WHEN("Renamed with changes to EB")
    {
      parameterUseCases.incDec(1, false, false);
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }
  }

  WHEN("Single Editbuffer converted to Split")
  {
    TestHelper::initSingleEditBuffer();
    ebUseCases.convertToSplit(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");

    WHEN("Renamed without changes to EB")
    {
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }

    WHEN("Renamed with changes to EB")
    {
      parameterUseCases.incDec(1, false, false);
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }
  }

  WHEN("Layer Editbuffer converted to Single")
  {
    TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
    ebUseCases.convertToSingle(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init conv.");
    CHECK(tmpPreset->getName() == "Init conv.");

    WHEN("Renamed without changes to EB")
    {
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }

    WHEN("Renamed with changes to EB")
    {
      parameterUseCases.incDec(1, false, false);
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }
  }

  WHEN("Layer Editbuffer converted to Split")
  {
    TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
    ebUseCases.convertToSingle(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init conv.");
    CHECK(tmpPreset->getName() == "Init conv.");

    WHEN("Renamed without changes to EB")
    {
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }

    WHEN("Renamed with changes to EB")
    {
      parameterUseCases.incDec(1, false, false);
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }
  }

  WHEN("Split Editbuffer converted to Layer")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    ebUseCases.convertToLayer(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init Sound");
    CHECK(tmpPreset->getName() == "Init Sound");

    WHEN("Renamed without changes to EB")
    {
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }

    WHEN("Renamed with changes to EB")
    {
      parameterUseCases.incDec(1, false, false);
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }
  }

  WHEN("Split Editbuffer converted to Single")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    ebUseCases.convertToSingle(VoiceGroup::I);
    presetUseCases.overwriteWithEditBuffer(eb);
    CHECK(eb.getName() == "Init conv.");
    CHECK(tmpPreset->getName() == "Init conv.");

    WHEN("Renamed without changes to EB")
    {
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }

    WHEN("Renamed with changes to EB")
    {
      parameterUseCases.incDec(1, false, false);
      presetUseCases.rename("Foo");
      CHECK(tmpPreset->getName() == "Foo");
      CHECK(eb.getName() == "Foo");
      auto newPreset = bankUseCases.appendEditBuffer();
      CHECK(newPreset->getName() == "Foo");
    }
  }
}