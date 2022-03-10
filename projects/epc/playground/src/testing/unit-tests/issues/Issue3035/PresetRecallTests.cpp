#include <testing/TestHelper.h>
#include "parameters/PitchbendParameter.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "device-settings/GlobalLocalEnableSetting.h"
#include "use-cases/SettingsUseCases.h"
#include "parameters/PedalParameter.h"
#include "use-cases/PedalParameterUseCases.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE("Issue 3035, Loading Preset with held bender leads to wrong values", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();

  ParameterId srcID = { C15::PID::Bender, VoiceGroup::Global };
  auto bender = eb->findAndCastParameterByID<PitchbendParameter>(srcID);
  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto connectionParam
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Bender_to_MC_A, VoiceGroup::Global });

  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  auto routings = settings->getSetting<RoutingSettings>();

  routings->setAllValues(true);
  PhysicalControlParameterUseCases benderUseCase(bender);
  benderUseCase.setDefault();

  const auto modTargetId = ParameterId({ C15::PID::Shp_A_Mix, VoiceGroup::I });
  auto modTarget = eb->findParameterByID(modTargetId);

  EditBufferUseCases ebUseCase(*eb);
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  MacroControlParameterUseCases mcUseCase(mcA);
  mcUseCase.setControlPosition(0.5);

  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  CHECK(modTarget->getControlPositionValue() == 0);

  WHEN("Bender is -1")
  {
    benderUseCase.setControlPosition(-1.);

    THEN("Mod Checks out")
    {
      TestHelper::doMainLoopIteration();
      CHECK(bender->getControlPositionValue() == -1);
      CHECK(mcA->getControlPositionValue() == 0);
      CHECK(modTarget->getControlPositionValue() == -1);
    }
  }

  WHEN("Bender is 1")
  {
    benderUseCase.setControlPosition(1.);

    THEN("Mod Checks out")
    {
      TestHelper::doMainLoopIteration();
      CHECK(bender->getControlPositionValue() == 1);
      CHECK(mcA->getControlPositionValue() == 1);
      CHECK(modTarget->getControlPositionValue() == 1);
    }
  }

  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  WHEN("Bender is 1 and Held while saving")
  {
    benderUseCase.setControlPosition(1.);
    TestHelper::doMainLoopIteration();

    auto bank = pmUseCases.createBankAndStoreEditBuffer();
    auto preset = bank->getPresetAt(0);

    THEN("EditBuffer Mod Checks out")
    {
      CHECK(bender->getControlPositionValue() == 1);
      CHECK(mcA->getControlPositionValue() == 1);
      CHECK(modTarget->getControlPositionValue() == 1);
    }

    THEN("Preset Mod Checks out -> Returning Sources (and consequent Parameters appear as modulated by the default "
         "return-target postion)")
    {
      auto presetBender = preset->findParameterByID(bender->getID(), true);
      auto presetMCA = preset->findParameterByID(mcA->getID(), true);
      auto presetModTarget = preset->findParameterByID(modTarget->getID(), true);
      CHECK(presetBender->getValue() == 0);
      CHECK(presetMCA->getValue() == 0.5);
      CHECK(presetModTarget->getValue() == 0);

      THEN("HW-Position is stuck at 1")
      {
        CHECK(bender->getControlPositionValue() == 1);
        CHECK(mcA->getControlPositionValue() == 1);
        CHECK(modTarget->getControlPositionValue() == 1);
      }

      WHEN("Bender is -1")
      {
        benderUseCase.setControlPosition(-1.);

        THEN("Mod Checks out")
        {
          TestHelper::doMainLoopIteration();
          CHECK(bender->getControlPositionValue() == -1);
          CHECK(mcA->getControlPositionValue() == 0);
          CHECK(modTarget->getControlPositionValue() == -1);
        }

        WHEN("Preset is loaded")
        {
          ebUseCase.load(preset);

          THEN("Bender is -1 and Mod Checks out")
          {
            TestHelper::doMainLoopIteration();
            CHECK(bender->getControlPositionValue() == -1);
            CHECK(mcA->getControlPositionValue() == 0);
            CHECK(modTarget->getControlPositionValue() == -1);
          }
        }
      }
    }
  }
}

TEST_CASE("Issue 3035, Loading Presets with differing ReturnTypes leads to correct loaded values", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();
  auto settings = TestHelper::getSettings();

  SettingsUseCases settingsUseCase(*settings);
  settingsUseCase.setAllRoutingEntries(true);
  settingsUseCase.setGlobalLocal(true);

  ParameterId srcID = { C15::PID::Pedal_4, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto connectionParam
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Pedal_4_to_MC_A, VoiceGroup::Global });

  PedalParameterUseCases pedalUseCase(pedal1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  const auto modTargetId = ParameterId({ C15::PID::Shp_A_Mix, VoiceGroup::I });
  auto modTarget = eb->findParameterByID(modTargetId);

  //Preset 1 mit Pedal 4 NonReturn auf 0% (standard)
  TestHelper::initSingleEditBuffer();
  //Macro Initial
  MacroControlParameterUseCases mcUseCase(mcA);
  mcUseCase.setControlPosition(0.5);

  //Pedal Initial
  pedalUseCase.setPedalMode(PedalModes::STAY);
  pedalUseCase.setControlPosition(0);
  CHECK(pedal1->getDisplayString() == "0.0 %");

  //Set Modulation Target
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  //Setup Mod routing
  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  CHECK(pedal1->getDisplayString() == "0.0 %");
  CHECK(modTarget->getDisplayString() == "0.0 %");
  auto preset1 = pmUseCases.createBankAndStoreEditBuffer()->getPresetAt(0);
  CHECK(pedal1->getDisplayString() == "0.0 %");

  //Preset 2 mit Pedal 4 ReturnZero auf 0%
  pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
  CHECK(pedal1->getDisplayString() == "0.0 %");
  CHECK(modTarget->getDisplayString() == "0.0 %");
  auto preset2 = pmUseCases.createBankAndStoreEditBuffer()->getPresetAt(0);
  CHECK(pedal1->getDisplayString() == "0.0 %");

  WHEN("Preset 1 is Loaded")
  {
    ebUseCase.load(preset1);
    CHECK(pedal1->getDisplayString() == "0.0 %");

    WHEN("Preset 2 is loaded")
    {
      ebUseCase.load(preset2);
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "0.0 %");
    }
  }
}