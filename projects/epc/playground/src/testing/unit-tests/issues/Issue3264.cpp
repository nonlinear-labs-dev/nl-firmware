#include <iostream>
#include <testing/TestHelper.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <parameter_declarations.h>
#include <use-cases/PresetUseCases.h>
#include <http/UndoScope.h>
#include "use-cases/DirectLoadUseCases.h"
#include "parameters/RibbonParameter.h"
#include "use-cases/RibbonParameterUseCases.h"
#include <CompileTimeOptions.h>
#include <presets/PresetParameter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "fresh Presets with all ribbons mapped")
{
  auto settings = TestHelper::getSettings();
  auto pm = TestHelper::getPresetManager();
  auto eb = pm->getEditBuffer();

  PresetManagerUseCases presetManagerUC(*pm, *settings);
  TestHelper::initSingleEditBuffer();

  for(auto ribId : { C15::PID::Ribbon_1, C15::PID::Ribbon_2, C15::PID::Ribbon_3, C15::PID::Ribbon_4 })
  {
    if(auto ribbon = eb->findAndCastParameterByID<RibbonParameter>({ ribId, VoiceGroup::Global }))
    {
      RibbonParameterUseCases ribbonUC(ribbon);
      ribbonUC.setReturnMode(RibbonReturnMode::STAY);
    }
  }

  for(auto& p : eb->getParameterGroupByID({ "MCM", VoiceGroup::Global })->getParameters())
  {
    ParameterUseCases puc(p);
    puc.setControlPosition(0);
  }

  for(auto id : { C15::PID::Ribbon_1_to_MC_A, C15::PID::Ribbon_2_to_MC_B, C15::PID::Ribbon_3_to_MC_E,
                  C15::PID::Ribbon_4_to_MC_F })
  {
    auto parameter = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases paramUC(parameter);
    paramUC.setControlPosition(1);
  }

  auto bankWithMappings = presetManagerUC.createBankAndStoreEditBuffer();
  auto presetWithMappings = bankWithMappings->getPresetAt(0);

  for(auto id : { C15::PID::Ribbon_1_to_MC_A, C15::PID::Ribbon_2_to_MC_B, C15::PID::Ribbon_3_to_MC_E,
                  C15::PID::Ribbon_4_to_MC_F })
  {
    auto parameter = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases paramUC(parameter);
    paramUC.setControlPosition(0);
  }

  auto bankWithoutMappings = presetManagerUC.createBankAndStoreEditBuffer();
  auto presetWithoutMappings = bankWithoutMappings->getPresetAt(0);

  THEN("Presets are correct")
  {
    for(auto id : { C15::PID::Aftertouch_to_MC_D, C15::PID::Bender_to_MC_D, C15::PID::Pedal_1_to_MC_D,
                    C15::PID::Pedal_2_to_MC_D, C15::PID::Pedal_3_to_MC_D, C15::PID::Pedal_4_to_MC_D,
                    C15::PID::Ribbon_1_to_MC_D, C15::PID::Ribbon_2_to_MC_D })
    {
      INFO("testing parameter " << id << " to not be mapped");
      CHECK_NOTHROW(presetWithMappings->findParameterByID({ id, VoiceGroup::Global }, true)->getValue() == 0);
      CHECK_NOTHROW(presetWithMappings->findParameterByID({ id, VoiceGroup::Global }, true)->getValue() == 0);
    }

    CHECK_NOTHROW(
        presetWithMappings->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global }, true)->getValue()
        == 1);
    CHECK_NOTHROW(
        presetWithMappings->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global }, true)->getValue()
        == 1);
    CHECK_NOTHROW(
        presetWithMappings->findParameterByID({ C15::PID::Ribbon_3_to_MC_E, VoiceGroup::Global }, true)->getValue()
        == 1);
    CHECK_NOTHROW(
        presetWithMappings->findParameterByID({ C15::PID::Ribbon_4_to_MC_F, VoiceGroup::Global }, true)->getValue()
        == 1);

    CHECK_NOTHROW(
        presetWithoutMappings->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global }, true)->getValue()
        == 0);
    CHECK_NOTHROW(
        presetWithoutMappings->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global }, true)->getValue()
        == 0);
    CHECK_NOTHROW(
        presetWithoutMappings->findParameterByID({ C15::PID::Ribbon_3_to_MC_E, VoiceGroup::Global }, true)->getValue()
        == 0);
    CHECK_NOTHROW(
        presetWithoutMappings->findParameterByID({ C15::PID::Ribbon_4_to_MC_F, VoiceGroup::Global }, true)->getValue()
        == 0);
  }

  EditBufferUseCases ebUseCases(*eb);

  WHEN("Initial load of presets")
  {
    ebUseCases.load(presetWithMappings);
    CHECK(eb->getOrigin() == presetWithMappings);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_3_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_4_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);

    ebUseCases.load(presetWithoutMappings);
    CHECK(eb->getOrigin() == presetWithoutMappings);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_3_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_4_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == presetWithMappings);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_3_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_4_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "Preset with E and F to ribbons, undoing loading other preset leads to same eb as explicit load")
{
  auto settings = TestHelper::getSettings();
  auto pm = TestHelper::getPresetManager();
  auto eb = pm->getEditBuffer();

  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
    CHECK(pm->getSelectedBank() == nullptr);
  }

  PresetManagerUseCases uc(*pm, *settings);
  FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/Issue-3264.xml", false);
  auto bank = uc.importBankFromStream(stream, 0, 0, "Issue-3264", Serializer::MockProgress);

  auto withMapping = bank->getPresetAt(0);
  auto withoutMapping = bank->getPresetAt(1);

  THEN("Presets are correct")
  {
    for(auto id : { C15::PID::Aftertouch_to_MC_D, C15::PID::Bender_to_MC_D, C15::PID::Pedal_1_to_MC_D,
                    C15::PID::Pedal_2_to_MC_D, C15::PID::Pedal_3_to_MC_D, C15::PID::Pedal_4_to_MC_D,
                    C15::PID::Ribbon_1_to_MC_D, C15::PID::Ribbon_2_to_MC_D })
    {
      INFO("testing parameter " << id << " to not be mapped");
      CHECK_NOTHROW(withMapping->findParameterByID({ id, VoiceGroup::Global }, true)->getValue() == 0);
      CHECK_NOTHROW(withoutMapping->findParameterByID({ id, VoiceGroup::Global }, true)->getValue() == 0);
    }

    CHECK_NOTHROW(withMapping->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global }, true)->getValue()
                  == 1);
    CHECK_NOTHROW(withMapping->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global }, true)->getValue()
                  == 1);

    CHECK_NOTHROW(
        withoutMapping->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global }, true)->getValue() == 0);
    CHECK_NOTHROW(
        withoutMapping->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global }, true)->getValue() == 0);
  }

  EditBufferUseCases ebUseCases(*eb);
  PresetManagerUseCases pmUseCases(*pm, *settings);

  ebUseCases.unlockAllGroups();

  WHEN("Initial load of presets")
  {
    ebUseCases.load(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);

    ebUseCases.load(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->isChangedFromLoaded() == false);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->isChangedFromLoaded() == false);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);
  }

  WHEN("Initial load of presets, reversed order")
  {
    ebUseCases.load(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);

    ebUseCases.load(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "Preset with A and B to ribbons, undoing loading other preset leads to same eb as explicit load")
{
  auto settings = TestHelper::getSettings();
  auto pm = TestHelper::getPresetManager();
  auto eb = pm->getEditBuffer();

  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
    CHECK(pm->getSelectedBank() == nullptr);
  }

  PresetManagerUseCases uc(*pm, *settings);
  FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/Issue-3264-2.xml", false);
  auto bank = uc.importBankFromStream(stream, 0, 0, "Issue-3264-2", Serializer::MockProgress);

  DirectLoadUseCases dlUseCase(settings->getSetting<DirectLoadSetting>());
  dlUseCase.setDirectLoad(true);

  auto withMapping = bank->getPresetAt(0);
  auto withoutMapping = bank->getPresetAt(1);

  THEN("Presets are correct")
  {
    CHECK(withMapping->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global }, true)->getValue() == 1);
    CHECK(withMapping->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global }, true)->getValue() == 1);

    CHECK(withoutMapping->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global }, true)->getValue() == 0);
    CHECK(withoutMapping->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global }, true)->getValue() == 0);
  }

  EditBufferUseCases ebUseCases(*eb);
  PresetManagerUseCases pmUseCases(*pm, *settings);

  WHEN("Initial load of presets")
  {
    ebUseCases.load(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);

    ebUseCases.load(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);
  }

  WHEN("Initial load of presets, reversed order")
  {
    ebUseCases.load(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);

    ebUseCases.load(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 1);
    CHECK(eb->findAnyParameterChanged() == false);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({ C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global })->getControlPositionValue() == 0);
    CHECK(eb->findAnyParameterChanged() == false);
  }
}