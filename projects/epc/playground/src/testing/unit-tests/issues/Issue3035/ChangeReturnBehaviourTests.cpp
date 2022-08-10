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

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Issue 3035, [Pedal] Set Return-Behaviour does lead to jump of value to return value but MC is same", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});
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
  ParameterUseCases sendUseCase(pedal1Send);
  sendUseCase.setControlPosition(0.5);

  WHEN("Pedal [STAY] Initial")
  {
    pedalUseCase.setPedalMode(PedalModes::STAY);
    pedalUseCase.setControlPosition(1);

    auto mcAPosition = mcA->getDisplayString();
    CHECK(mcAPosition == "100.0 %");
    CHECK(pedal1->getDisplayString() == "100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");

    WHEN("pedal is set to return to zero")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
      TestHelper::doMainLoopIteration();

      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }

    WHEN("pedal is set to return to center")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
      TestHelper::doMainLoopIteration();

      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }
  }

  WHEN("Pedal [Return to Center] Initial")
  {
    pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
    pedalUseCase.setControlPosition(1);

    auto mcAPosition = mcA->getDisplayString();
    CHECK(mcAPosition == "100.0 %");
    CHECK(pedal1->getDisplayString() == "! 100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");

    WHEN("pedal is set to return to Stay")
    {
      pedalUseCase.setPedalMode(PedalModes::STAY);
      CHECK(pedal1->getDisplayString() == "100.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }

    WHEN("pedal is set to return to Zero")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }
  }

  WHEN("Pedal [Return To Zero] Initial")
  {
    pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
    pedalUseCase.setControlPosition(1);

    auto mcAPosition = mcA->getDisplayString();
    CHECK(mcAPosition == "100.0 %");
    CHECK(pedal1->getDisplayString() == "! 100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");

    WHEN("pedal is set to return to Stay")
    {
      pedalUseCase.setPedalMode(PedalModes::STAY);
      CHECK(pedal1->getDisplayString() == "100.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }

    WHEN("pedal is set to return to Center")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Issue 3035, [Ribbon] Set Return-Behaviour does lead to jump of value to return value but MC is same", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});
  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto connectionParam
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global });

  RibbonParameterUseCases ribbonUseCases(ribbon1);
  EditBufferUseCases ebUseCase(*eb);
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  const auto modTargetId = ParameterId({ C15::PID::Shp_A_Mix, VoiceGroup::I });
  auto modTarget = eb->findParameterByID(modTargetId);

  TestHelper::initSingleEditBuffer();

  //Local On
  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  //Macro Initial
  MacroControlParameterUseCases mcUseCase(mcA);
  mcUseCase.setControlPosition(0.5);

  //Set Modulation Target
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  CHECK(mcA->getDisplayString() == "50.0 %");
  CHECK(modTarget->getDisplayString() == "0.0 %");

  //Setup Mod routing
  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  //Send Parameter useCase
  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);

  WHEN("Ribbon [STAY] Initial")
  {
    ribbonUseCases.setReturnMode(RibbonReturnMode::STAY);
    ribbonUseCases.setControlPosition(1);
    auto oldMCPos = mcA->getDisplayString();
    auto oldModPos = modTarget->getDisplayString();

    CHECK(ribbon1->getDisplayString() == "100.0 %");
    CHECK(oldMCPos == "100.0 %");
    CHECK(oldModPos == "100.0 %");

    WHEN("Ribbon is set to return")
    {
      ribbonUseCases.setReturnMode(RibbonReturnMode::RETURN);
      CHECK(ribbon1->getDisplayString() == "0.0 %");
      CHECK(mcA->getDisplayString() == oldMCPos);
      CHECK(modTarget->getDisplayString() == oldModPos);
    }
  }

  WHEN("Ribbon [Return] Initial")
  {
    routeUseCase.setControlPosition(1);

    ribbonUseCases.setReturnMode(RibbonReturnMode::RETURN);
    WHEN("pos is 1")
    {
      ribbonUseCases.setControlPosition(1);
      auto oldMCPos = mcA->getDisplayString();
      auto oldModPos = modTarget->getDisplayString();

      CHECK(ribbon1->getDisplayString() == "! 100.0 %");
      CHECK(oldMCPos == "100.0 %");
      CHECK(oldModPos == "100.0 %");

      WHEN("Ribbon is set to Stay")
      {
        ribbonUseCases.setReturnMode(RibbonReturnMode::STAY);
        CHECK(ribbon1->getDisplayString() == "100.0 %");
        CHECK(mcA->getDisplayString() == oldMCPos);
        CHECK(modTarget->getDisplayString() == oldModPos);
      }
    }

    WHEN("pos is -1")
    {
      ribbonUseCases.setControlPosition(-1);
      auto oldMCPos = mcA->getDisplayString();
      auto oldModPos = modTarget->getDisplayString();

      CHECK(ribbon1->getDisplayString() == "! -100.0 %");
      CHECK(oldMCPos == "0.0 %");
      CHECK(oldModPos == "-100.0 %");

      WHEN("Ribbon is set to Stay")
      {
        ribbonUseCases.setReturnMode(RibbonReturnMode::STAY);
        CHECK(ribbon1->getDisplayString() == "0.0 %");
        CHECK(mcA->getDisplayString() == oldMCPos);
        CHECK(modTarget->getDisplayString() == oldModPos);
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Issue 3035, [Pedal] Set Return-Behaviour (non-ret to ret) leads to HW-Amount set to 100%", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});
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
  ParameterUseCases sendUseCase(pedal1Send);
  sendUseCase.setControlPosition(0.5);

  pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
  pedalUseCase.changeFromAudioEngine(0.2, HWChangeSource::TCD);

  constexpr auto modRoutingValue = 1;
  routeUseCase.setControlPosition(modRoutingValue);

  WHEN("Pedal [STAY] Initial")
  {
    pedalUseCase.setPedalMode(PedalModes::STAY);
    pedalUseCase.setControlPosition(1);

    WHEN("pedal is set to return to zero")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
      CHECK(connectionParam->getControlPositionValue() == 1);
    }

    WHEN("pedal is set to return to center")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
      CHECK(connectionParam->getControlPositionValue() == 1);
    }
  }

  WHEN("Pedal [Return to Center] Initial")
  {
    pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
    pedalUseCase.setControlPosition(1);

    auto mcAPosition = mcA->getDisplayString();
    CHECK(mcAPosition == "100.0 %");
    CHECK(pedal1->getDisplayString() == "! 100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");

    WHEN("pedal is set to return to Stay")
    {
      pedalUseCase.setPedalMode(PedalModes::STAY);
      CHECK(pedal1->getDisplayString() == "100.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }

    WHEN("pedal is set to return to Zero")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }
  }

  WHEN("Pedal [Return To Zero] Initial")
  {
    pedalUseCase.setPedalMode(PedalModes::RETURN_TO_ZERO);
    pedalUseCase.setControlPosition(1);

    auto mcAPosition = mcA->getDisplayString();
    CHECK(mcAPosition == "100.0 %");
    CHECK(pedal1->getDisplayString() == "! 100.0 %");
    CHECK(modTarget->getDisplayString() == "100.0 %");

    WHEN("pedal is set to return to Stay")
    {
      pedalUseCase.setPedalMode(PedalModes::STAY);
      CHECK(pedal1->getDisplayString() == "100.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }

    WHEN("pedal is set to return to Center")
    {
      pedalUseCase.setPedalMode(PedalModes::RETURN_TO_CENTER);
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(modTarget->getDisplayString() == "100.0 %");
      CHECK(mcA->getDisplayString() == mcAPosition);
    }
  }
}