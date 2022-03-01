#include <testing/TestHelper.h>
#include "parameters/PitchbendParameter.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "parameters/PedalParameter.h"
#include "use-cases/PedalParameterUseCases.h"
#include "parameters/RibbonParameter.h"
#include "use-cases/RibbonParameterUseCases.h"
#include "device-settings/GlobalLocalEnableSetting.h"
#include "use-cases/SettingsUseCases.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE("Issue 3035, Loading Preset with held bender leads to wrong values")
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

  const auto modTargetId = ParameterId({C15::PID::Shp_A_Mix, VoiceGroup::I});
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

    THEN("Preset Mod Checks out -> Returning Sources (and consequent Parameters appear as modulated by the default return-target postion)")
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

// Preset 1 mit Pedal 4 NonReturn auf 0% (standard), Preset 2 mit Pedal 4 ReturnZero auf 0%, wenn Preset 2 nach 1 geladen wird, stehen nach Recall die ReturnZero Source bei 50% (statt auf 0%), und mit ihr der Macro und die Target Parameter
TEST_CASE("Issue 3035, Loading Presets with differing ReturnTypes leads to correct loaded values")
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

  const auto modTargetId = ParameterId({C15::PID::Shp_A_Mix, VoiceGroup::I});
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


//- On -> Off bei einer returnenden Source: Send erhält aktuelle Position, Receive geht auf Return Position
TEST_CASE("Issue 3035, Pedal [Returning] Local On to Local Off")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(pedal1Send);
  sendUseCase.setControlPosition(0.5);

  pedalUseCases.setPedalMode(PedalModes::RETURN_TO_CENTER);
  pedalUseCases.setControlPosition(1);
  CHECK(pedal1->getDisplayString() == "! 100.0 %");
  auto oldCP = pedal1->getControlPositionValue();
  auto oldDisplayString = pedal1->getDisplayString();

  CHECK(pedal1->getDisplayString() == "! 100.0 %");
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, false);

    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> return position, send to oldCP")
    {
      CHECK(pedal1Send->getControlPositionValue() == oldCP);
      CHECK(pedal1->getControlPositionValue() == 0);
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Returning] Local On to Local Off")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});

  RibbonParameterUseCases ribbonUseCases(ribbon1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);

  ribbonUseCases.setReturnMode(RibbonReturnMode::RETURN);
  ribbonUseCases.setControlPosition(1);

  CHECK(ribbon1->getDisplayString() == "! 100.0 %");
  CHECK(ribbon1Send->getDisplayString() == "0.0 %");

  auto oldCP = ribbon1->getControlPositionValue();
  auto oldDisplayString = ribbon1->getDisplayString();


  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, false);
    TestHelper::doMainLoopIteration();

    THEN("Ribbon Parameter is set to return position, Send Parameter is set to oldCP")
    {
      CHECK(ribbon1Send->getControlPositionValue() == oldCP);
      CHECK(ribbon1->getControlPositionValue() == 0);
      CHECK(ribbon1->getDisplayString() == "0.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Returning] Local Off to Local On")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});

  RibbonParameterUseCases ribbonUseCase(ribbon1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  TestHelper::initSingleEditBuffer();

  //Local Off
  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, false);
  TestHelper::doMainLoopIteration();

  //Send Parameter useCase
  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);

  //Ribbon Initial
  ribbonUseCase.setReturnMode(RibbonReturnMode::RETURN);
  ribbonUseCase.setControlPosition(1);
  CHECK(ribbon1->getDisplayString() == "! 100.0 %");
  auto oldCP = ribbon1->getControlPositionValue();
  auto oldDisplayString = ribbon1->getDisplayString();

  CHECK(ribbon1->getDisplayString() == "! 100.0 %");
  CHECK(ribbon1Send->getDisplayString() == "0.0 %");

  WHEN("Local is set to On")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, true);
    TestHelper::doMainLoopIteration();

    THEN("Ribbon Parameter RVC -> current pos, send to returnPos")
    {
      CHECK(ribbon1->getDisplayString() == "0.0 %");
      CHECK(ribbon1Send->getDisplayString() == "100.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Pedal [Returning] Local Off to Local On")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  TestHelper::initSingleEditBuffer();

  //Local Off
  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, false);
  TestHelper::doMainLoopIteration();

  //Send Parameter useCase
  ParameterUseCases sendUseCase(pedal1Send);
  sendUseCase.setControlPosition(0.5);

  //Pedal Initial
  pedalUseCases.setPedalMode(PedalModes::RETURN_TO_ZERO);
  pedalUseCases.setControlPosition(1);
  CHECK(pedal1->getDisplayString() == "! 100.0 %");
  auto oldCP = pedal1->getControlPositionValue();
  auto oldDisplayString = pedal1->getDisplayString();

  CHECK(pedal1->getDisplayString() == "! 100.0 %");
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to On")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> current pos, send to returnPos")
    {
      CHECK(pedal1->getControlPositionValue() == oldCP);
      CHECK(pedal1Send->getValue().isBiPolar() == false);
      CHECK(pedal1Send->getDisplayString() == "0.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Stay] Local Off to Local On -> Set Ribbon Pos to mapped MC position")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});
  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto connectionParam
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global });

  RibbonParameterUseCases ribbonUseCase(ribbon1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  const auto modTargetId = ParameterId({ C15::PID::Shp_A_Mix, VoiceGroup::I });
  auto modTarget = eb->findParameterByID(modTargetId);

  TestHelper::initSingleEditBuffer();

  //Local Off
  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, false);
  TestHelper::doMainLoopIteration();

  //Macro Initial
  MacroControlParameterUseCases mcUseCase(mcA);
  mcUseCase.setControlPosition(0.5);

  //Set Modulation Target
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  //Setup Mod routing
  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  //Send Parameter useCase
  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);

  //Ribbon Initial
  ribbonUseCase.setReturnMode(RibbonReturnMode::STAY);
  ribbonUseCase.setControlPosition(0.5);
  CHECK(ribbon1->getDisplayString() == "50.0 %");
  auto oldDisplayString = ribbon1->getDisplayString();

  CHECK(ribbon1->getDisplayString() == oldDisplayString);
  CHECK(modTarget->getDisplayString() == "0.0 %");

  WHEN("MC A set to 100%")
  {
    mcUseCase.setControlPosition(1);
    CHECK(modTarget->getDisplayString() == "100.0 %");
    CHECK(mcA->getDisplayString() == "100.0 %");
    CHECK(ribbon1->getDisplayString() == "100.0 %");
    CHECK(ribbon1Send->getDisplayString() == "50.0 %");

    WHEN("Local is set to On")
    {
      settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
      settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, true);
      TestHelper::doMainLoopIteration();

      CHECK(ribbon1->getControlPositionValue() == mcA->getControlPositionValue());
    }
  }
}

TEST_CASE("Issue 3035, Set Return-Behaviour does not lead to jump of value to return value")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto ribbon1Send = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});
  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto connectionParam
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Pedal_1_to_MC_A, VoiceGroup::Global });

  PedalParameterUseCases pedalUseCase(pedal1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  const auto modTargetId = ParameterId({ C15::PID::Shp_A_Mix, VoiceGroup::I });
  auto modTarget = eb->findParameterByID(modTargetId);

  TestHelper::initSingleEditBuffer();

  //Local On
  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  //Macro Initial
  MacroControlParameterUseCases mcUseCase(mcA);
  mcUseCase.setControlPosition(0.5);

  //Set Modulation Target
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  //Setup Mod routing
  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  //Send Parameter useCase
  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);

  //Pedal Initial
  pedalUseCase.setPedalMode(PedalModes::STAY);
  pedalUseCase.setControlPosition(1);
  CHECK(pedal1->getDisplayString() == "100.0 %");
  CHECK(modTarget->getDisplayString() == "100.0 %");

  WHEN("pedal is set to return to zero")
  {
    pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
    CHECK(pedal1->getDisplayString() == "! 100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");
  }

  WHEN("pedal is set to return to center")
  {
    pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
    CHECK(pedal1->getDisplayString() == "! 100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");
  }
}