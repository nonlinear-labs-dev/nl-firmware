#include <testing/TestHelper.h>
#include "parameters/PitchbendParameter.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "device-settings/GlobalLocalEnableSetting.h"
#include "use-cases/SettingsUseCases.h"
#include "parameters/PedalParameter.h"
#include "use-cases/PedalParameterUseCases.h"
#include "CompileTimeOptions.h"
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

TEST_CASE("Issue 3035, Loading Presets with differing ReturnTypes leads to correct loaded values Pedal Return to Zero", "[3035]")
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
  pedalUseCase.setControlPosition(0.5);
  CHECK(pedal1->getDisplayString() == "50.0 %");

  //Set Modulation Target
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  //Setup Mod routing
  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  REQUIRE(pedal1->getDisplayString() == "50.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");

  auto mcAPreset1Pos = mcA->getDisplayString();
  auto preset1 = pmUseCases.createBankAndStoreEditBuffer()->getPresetAt(0);

  REQUIRE(pedal1->getDisplayString() == "50.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");

  pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");

  auto mcAPreset2Pos = mcA->getDisplayString();
  auto preset2 = pmUseCases.createBankAndStoreEditBuffer()->getPresetAt(0);

  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");
  REQUIRE(pedal1->getControlPositionValue() == 0);

  ebUseCase.load(preset1);
  REQUIRE(pedal1->getControlPositionValue() == 0);
  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == mcAPreset1Pos);

  ebUseCase.load(preset2);
  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == mcAPreset2Pos);
}


TEST_CASE("Issue 3035, Loading Presets with differing ReturnTypes leads to correct loaded values Pedal Return to Center", "[3035]")
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
  pedalUseCase.setControlPosition(0.5);
  CHECK(pedal1->getDisplayString() == "50.0 %");

  //Set Modulation Target
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  //Setup Mod routing
  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  REQUIRE(pedal1->getDisplayString() == "50.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");

  auto mcAPreset1Pos = mcA->getDisplayString();
  auto preset1 = pmUseCases.createBankAndStoreEditBuffer()->getPresetAt(0);

  REQUIRE(pedal1->getDisplayString() == "50.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");

  pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");

  auto mcAPreset2Pos = mcA->getDisplayString();
  auto preset2ReturnToCenter = pmUseCases.createBankAndStoreEditBuffer()->getPresetAt(0);

  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == "50.0 %");
  REQUIRE(pedal1->getControlPositionValue() == 0);

  ebUseCase.load(preset1);
  REQUIRE(pedal1->getControlPositionValue() == 0);
  REQUIRE(pedal1->getDisplayString() == "0.0 %");
  REQUIRE(mcA->getDisplayString() == mcAPreset1Pos);

  pedalUseCase.changeFromAudioEngine(0.5, HWChangeSource::TCD);

  ebUseCase.load(preset2ReturnToCenter);
  REQUIRE(pedal1->getControlPositionValue() == 0.5);
  REQUIRE(pedal1->getDisplayString() == "! 50.0 %");
  REQUIRE(mcAPreset2Pos == "100.0 %");
  REQUIRE(modTarget->getDisplayString() == "0.0 %");
}

TEST_CASE("Load Preset with differing Return Types", "[3035]")
{
  auto pm = TestHelper::getPresetManager();
  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
  }

  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);

  auto bank = uc.importBankFromPath(std::filesystem::directory_entry{getSourceDir() + "/projects/epc/playground/test-resources/3035-5.xml"}, [](auto){});
  auto init = bank->getPresetAt(0);
  auto ped1_0 = bank->getPresetAt(1);
  auto PB_AT = bank->getPresetAt(2);
  auto PB_AT_2 = bank->getPresetAt(3);
  auto ped1_nonret = bank->getPresetAt(4);
  auto ped1_retzero = bank->getPresetAt(5);
  auto ped1_retcenter = bank->getPresetAt(6);
  auto ped4_nonret = bank->getPresetAt(7);
  auto ped4_retzero = bank->getPresetAt(8);
  auto ped4_retzero2 = bank->getPresetAt(9);
  auto rib1_stay = bank->getPresetAt(10);
  auto rib1_retcenter = bank->getPresetAt(12);
  auto rib1_retcenter_2 = bank->getPresetAt(13);
}