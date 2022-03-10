#include <testing/TestHelper.h>
#include "parameters/PitchbendParameter.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "device-settings/GlobalLocalEnableSetting.h"
#include "use-cases/SettingsUseCases.h"
#include "parameters/PedalParameter.h"
#include "use-cases/PedalParameterUseCases.h"
#include "parameters/RibbonParameter.h"
#include "use-cases/RibbonParameterUseCases.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE("Issue 3035, Pedal [Returning Zero] Local Off to Local On", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
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
      CHECK(pedal1->getDisplayString() == "50.0 %");
      CHECK(pedal1Send->getDisplayString() == "0.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Pedal [Returning Center] Local Off to Local On", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
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
  pedalUseCases.setPedalMode(PedalModes::RETURN_TO_CENTER);
  pedalUseCases.setControlPosition(1);

  CHECK(pedal1->getDisplayString() == "! 100.0 %");
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to On")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> current pos, send to returnPos")
    {
      CHECK(pedal1->getDisplayString() == "50.0 %");
      CHECK(pedal1Send->getDisplayString() == "0.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Pedal [Nonreturn] Local Off to Local On", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
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
  pedalUseCases.setPedalMode(PedalModes::STAY);
  pedalUseCases.setControlPosition(1);

  CHECK(pedal1->getDisplayString() == "100.0 %");
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to On")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> current pos, send to returnPos")
    {
      CHECK(pedal1->getDisplayString() == "100.0 %");
      CHECK(pedal1Send->getDisplayString() == "50.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Returning] Local Off to Local On", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});

  RibbonParameterUseCases ribbonUseCase(ribbon1);
  TestHelper::initSingleEditBuffer();

  //Local Off
  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, false);
  TestHelper::doMainLoopIteration();

  //Send Parameter useCase
  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);
  CHECK(ribbon1Send->getControlPositionValue() == 0.5);
  CHECK(ribbon1Send->getDisplayString() == "50.0 %");

  //Ribbon Initial
  ribbonUseCase.setReturnMode(RibbonReturnMode::RETURN);
  ribbonUseCase.setControlPosition(1);
  CHECK(ribbon1->getDisplayString() == "! 100.0 %");

  CHECK(ribbon1Send->getControlPositionValue() == 0.5);
  CHECK(ribbon1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to On")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, true);
    TestHelper::doMainLoopIteration();

    THEN("Ribbon Parameter RVC -> current pos, send to returnPos")
    {
      CHECK(ribbon1->getDisplayString() == "50.0 %");
      CHECK(ribbon1Send->getDisplayString() == "0.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Stay] Local Off to Local On -> Set Ribbon Pos to mapped MC position", "[3035]")
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

TEST_CASE("Issue 3035, Bender Local Off to Local On", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Bender, VoiceGroup::Global };
  auto bender = eb->findAndCastParameterByID<PitchbendParameter>(srcID);
  auto benderSend
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Bender_Send, VoiceGroup::Global});

  ParameterUseCases benderUseCases(bender);

  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Bender, RoutingSettings::tAspectIndex::LOCAL, false);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(benderSend);

  benderUseCases.setControlPosition(1);
  sendUseCase.setControlPosition(0.5);

  CHECK(bender->getDisplayString() == "! 100.0 %");
  CHECK(benderSend->getDisplayString() == "50.0 %");

  WHEN("Local is set to On")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Bender, RoutingSettings::tAspectIndex::LOCAL, true);
    TestHelper::doMainLoopIteration();

    THEN("Bender has current position and send is reset to return value")
    {
      CHECK(bender->getDisplayString() == "50.0 %");
      CHECK(benderSend->getDisplayString() == "0.0 %");
    }
  }
}