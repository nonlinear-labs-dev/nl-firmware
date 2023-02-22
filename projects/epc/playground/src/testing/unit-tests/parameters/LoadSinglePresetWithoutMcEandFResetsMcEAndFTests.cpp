#include <testing/unit-tests/mock/PreDualModePresetBank.h>
#include <parameter_declarations.h>
#include "testing/TestHelper.h"
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include "presets/Preset.h"
#include "presets/PresetParameter.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/ModulateableParameter.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Load Single Preset Without MacroControl E and F defaults MC E and F")
{
  auto mockPreset = PreDualModePresetBank::createMockPreset();

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  TestHelper::initSingleEditBuffer();

  auto mcE = dynamic_cast<MacroControlParameter*>(eb->findParameterByID({ C15::PID::MC_E, VoiceGroup::Global }));
  auto mcF = dynamic_cast<MacroControlParameter*>(eb->findParameterByID({ C15::PID::MC_F, VoiceGroup::Global }));

  auto partVolume
      = dynamic_cast<ModulateableParameter*>(eb->findParameterByID({ C15::PID::Part_Volume, VoiceGroup::I }));
  auto partTune = dynamic_cast<ModulateableParameter*>(eb->findParameterByID({ C15::PID::Part_Tune, VoiceGroup::I }));

  {
    auto scope = TestHelper::createTestScope();
    partVolume->setModulationSource(scope->getTransaction(), MacroControls::MC5);
    partVolume->setModulationAmount(scope->getTransaction(), 0.2);
    partTune->setModulationSource(scope->getTransaction(), MacroControls::MC6);
    partTune->setModulationAmount(scope->getTransaction(), 0.4);

    mcE->undoableSetGivenName(scope->getTransaction(), "EE");
    mcF->undoableSetGivenName(scope->getTransaction(), "FF");
    mcE->setCPFromHwui(scope->getTransaction(), 0.75);
    mcF->setCPFromHwui(scope->getTransaction(), 0.25);
  }

  THEN("Loading old Preset resets non existing parameters")
  {
    useCase.load(mockPreset.get());
    CHECK(mcE->getGivenName().empty());
    CHECK(mcF->getGivenName().empty());
    CHECK(partTune->getModulationSource() == MacroControls::NONE);
    CHECK(partVolume->getModulationSource() == MacroControls::NONE);
    CHECK(partTune->getModulationAmount() == Approx(0.0));
    CHECK(partVolume->getModulationAmount() == Approx(0.0));
  }
}
