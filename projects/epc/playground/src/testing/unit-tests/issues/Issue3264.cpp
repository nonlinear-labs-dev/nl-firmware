#include <iostream>
#include <testing/TestHelper.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <parameter_declarations.h>
#include <use-cases/PresetUseCases.h>
#include <http/UndoScope.h>
#include "use-cases/DirectLoadUseCases.h"
#include <CompileTimeOptions.h>
#include <presets/PresetParameter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Preset with E and F to ribbons, undoing loading other preset leads to same eb as explicit load")
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

  DirectLoadUseCases dlUseCase(settings->getSetting<DirectLoadSetting>());
  dlUseCase.setDirectLoad(true);

  auto withMapping = bank->getPresetAt(0);
  auto withoutMapping = bank->getPresetAt(1);

  THEN("Presets are correct")
  {
    CHECK(withMapping->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global}, true)->getValue() == 1);
    CHECK(withMapping->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global}, true)->getValue() == 1);

    CHECK(withoutMapping->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global}, true)->getValue() == 0);
    CHECK(withoutMapping->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global}, true)->getValue() == 0);
  }

  EditBufferUseCases ebUseCases(*eb);
  PresetManagerUseCases pmUseCases(*pm, *settings);

  WHEN("Initial load of presets")
  {
    pmUseCases.selectPreset(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global})->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global})->getControlPositionValue() == 1);

    pmUseCases.selectPreset(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global})->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global})->getControlPositionValue() == 0);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global})->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global})->getControlPositionValue() == 1);
  }

  WHEN("Initial load of presets, reversed order")
  {
    pmUseCases.selectPreset(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global})->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global})->getControlPositionValue() == 0);

    pmUseCases.selectPreset(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global})->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global})->getControlPositionValue() == 1);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_E, VoiceGroup::Global})->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_F, VoiceGroup::Global})->getControlPositionValue() == 0);
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Preset with A and B to ribbons, undoing loading other preset leads to same eb as explicit load")
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
    CHECK(withMapping->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global}, true)->getValue() == 1);
    CHECK(withMapping->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global}, true)->getValue() == 1);

    CHECK(withoutMapping->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global}, true)->getValue() == 0);
    CHECK(withoutMapping->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global}, true)->getValue() == 0);
  }

  EditBufferUseCases ebUseCases(*eb);
  PresetManagerUseCases pmUseCases(*pm, *settings);

  WHEN("Initial load of presets")
  {
    pmUseCases.selectPreset(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global})->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global})->getControlPositionValue() == 1);

    pmUseCases.selectPreset(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global})->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global})->getControlPositionValue() == 0);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global})->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global})->getControlPositionValue() == 1);
  }

  WHEN("Initial load of presets, reversed order")
  {
    pmUseCases.selectPreset(withoutMapping);
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global})->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global})->getControlPositionValue() == 0);

    pmUseCases.selectPreset(withMapping);
    CHECK(eb->getOrigin() == withMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global})->getControlPositionValue() == 1);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global})->getControlPositionValue() == 1);

    Application::get().getUndoScope()->undo();
    CHECK(eb->getOrigin() == withoutMapping);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global})->getControlPositionValue() == 0);
    CHECK(eb->findParameterByID({C15::PID::Ribbon_2_to_MC_B, VoiceGroup::Global})->getControlPositionValue() == 0);
  }
}