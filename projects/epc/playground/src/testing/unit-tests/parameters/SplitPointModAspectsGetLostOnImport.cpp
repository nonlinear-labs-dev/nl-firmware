#include "testing/TestHelper.h"
#include "presets/Preset.h"
#include "presets/PresetManager.h"
#include <serialization/PresetManagerSerializer.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "CompileTimeOptions.h"
#include <xml/ZippedMemoryOutStream.h>
#include <parameter_declarations.h>
#include <presets/PresetParameter.h>
#include <parameters/SplitPointParameter.h>

TEST_CASE("Import Preset with ModAspects on Split")
{
  auto pm = TestHelper::getPresetManager();

  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
    CHECK(pm->getSelectedBank() == nullptr);
  }

  PresetManagerUseCases uc(pm);
  FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/Fuxi_Split.xml", false);
  uc.importBankFromStream(stream, 0, 0, "Fuxi-Split", Serializer::Progress{});

  auto fuxiesSplit = pm->getSelectedBank();
  CHECK(fuxiesSplit != nullptr);

  auto firstPreset = fuxiesSplit->getPresetAt(0);
  CHECK(firstPreset != nullptr);

  auto oldParam = firstPreset->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::Global }, false);
  CHECK(oldParam == nullptr);

  auto splitIDI = ParameterId{ C15::PID::Split_Split_Point, VoiceGroup::I };
  auto splitIDII = ParameterId{ C15::PID::Split_Split_Point, VoiceGroup::II };
  auto splitPntI = firstPreset->findParameterByID(splitIDI, false);
  auto splitPntII = firstPreset->findParameterByID(splitIDII, false);
  CHECK(splitPntI != nullptr);
  CHECK(splitPntII != nullptr);

  THEN("Modulation Aspects are Present")
  {
    CHECK(splitPntI->getModulationAmount() != 0);
    CHECK(splitPntII->getModulationAmount() != 0);
    CHECK(splitPntI->getModulationSource() != MacroControls::NONE);
    CHECK(splitPntII->getModulationSource() != MacroControls::NONE);
  }

  WHEN("Preset is loaded")
  {
    EditBufferUseCases useCase(*TestHelper::getEditBuffer());
    useCase.load(firstPreset);

    THEN("Mod Aspects are present")
    {
      auto eb = TestHelper::getEditBuffer();
      auto sI = eb->findAndCastParameterByID<SplitPointParameter>(splitIDI);
      auto sII = eb->findAndCastParameterByID<SplitPointParameter>(splitIDII);
      CHECK(sI->getModulationSource() != MacroControls::NONE);
      CHECK(sII->getModulationSource() != MacroControls::NONE);
      CHECK(sI->getModulationAmount() != 0);
      CHECK(sII->getModulationAmount() != 0);
    }
  }
}

TEST_CASE("Import Conversion for Bank with Version 7 Checks out")
{
  auto pm = TestHelper::getPresetManager();

  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
    CHECK(pm->getSelectedBank() == nullptr);
  }

  PresetManagerUseCases uc(pm);
  FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/Version7Split.xml", false);
  uc.importBankFromStream(stream, 0, 0, "Split7", Serializer::Progress{});

  auto newBank = pm->getSelectedBank();
  CHECK(newBank != nullptr);

  auto newPreset = newBank->getPresetAt(0);
  CHECK(newPreset != nullptr);

  auto oldParam = newPreset->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::Global }, false);
  CHECK(oldParam == nullptr);

  auto splitIDI = ParameterId{ C15::PID::Split_Split_Point, VoiceGroup::I };
  auto splitIDII = ParameterId{ C15::PID::Split_Split_Point, VoiceGroup::II };
  auto splitPntI = newPreset->findParameterByID(splitIDI, false);
  auto splitPntII = newPreset->findParameterByID(splitIDII, false);
  CHECK(splitPntI != nullptr);
  CHECK(splitPntII != nullptr);

  constexpr auto splitIIExpectedValue = 0.5 + (1.0 / 60.0);

  THEN("CP Positions are correct")
  {
    CHECK(splitPntI->getValue() == 0.5);
    CHECK(splitPntII->getValue() == splitIIExpectedValue);
  }

  WHEN("Preset is loaded")
  {
    EditBufferUseCases useCase(*TestHelper::getEditBuffer());
    useCase.load(newPreset);

    THEN("Values are as expected")
    {
      auto eb = TestHelper::getEditBuffer();
      auto sI = eb->findAndCastParameterByID<SplitPointParameter>(splitIDI);
      auto sII = eb->findAndCastParameterByID<SplitPointParameter>(splitIDII);
      CHECK(sI->getControlPositionValue() == 0.5);
      CHECK(sII->getControlPositionValue() == splitIIExpectedValue);
    }
  }
}