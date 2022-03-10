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

TEST_CASE("Issue 3035, Pedal [Stay] Local On to Local Off", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(pedal1Send);
  sendUseCase.setControlPosition(0.5);

  pedalUseCases.setPedalMode(PedalModes::STAY);
  pedalUseCases.setControlPosition(1);

  CHECK(pedal1->getDisplayString() == "100.0 %");
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, false);

    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> return position, send to oldCP")
    {
      CHECK(pedal1->getDisplayString() == "100.0 %");
      CHECK(pedal1Send->getDisplayString() == "100.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Pedal [Returning Zero] Local On to Local Off", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(pedal1Send);
  sendUseCase.setControlPosition(0.5);

  pedalUseCases.setPedalMode(PedalModes::RETURN_TO_ZERO);
  pedalUseCases.setControlPosition(1);
  CHECK(pedal1->getDisplayString() == "! 100.0 %");
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, false);

    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> return position, send to oldCP")
    {
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(pedal1Send->getDisplayString() == "100.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Pedal [Returning Center] Local On to Local Off", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Pedal_1, VoiceGroup::Global };
  auto pedal1 = eb->findAndCastParameterByID<PedalParameter>(srcID);
  auto pedal1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Pedal_1_Send, VoiceGroup::Global});

  PedalParameterUseCases pedalUseCases(pedal1);
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
  CHECK(pedal1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Pedal1, RoutingSettings::tAspectIndex::LOCAL, false);

    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> return position, send to oldCP")
    {
      CHECK(pedal1->getControlPositionValue() == 0);
      CHECK(pedal1->getDisplayString() == "0.0 %");
      CHECK(pedal1Send->getDisplayString() == "100.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Returning] Local On to Local Off", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});

  RibbonParameterUseCases ribbonUseCases(ribbon1);
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
  CHECK(ribbon1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, false);
    TestHelper::doMainLoopIteration();

    THEN("Ribbon Parameter is set to return position, Send Parameter is set to oldCP")
    {
      CHECK(ribbon1Send->getDisplayString() == "100.0 %");
      CHECK(ribbon1->getDisplayString() == "0.0 %");
    }
  }
}

TEST_CASE("Issue 3035, Ribbon [Stay] Local On to Local Off", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Ribbon_1, VoiceGroup::Global };
  auto ribbon1 = eb->findAndCastParameterByID<RibbonParameter>(srcID);
  auto ribbon1Send
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Ribbon_1_Send, VoiceGroup::Global});

  RibbonParameterUseCases ribbonUseCases(ribbon1);
  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(ribbon1Send);
  sendUseCase.setControlPosition(0.5);


  ribbonUseCases.setReturnMode(RibbonReturnMode::STAY);
  ribbonUseCases.setControlPosition(1);
  CHECK(ribbon1->getDisplayString() == "100.0 %");
  CHECK(ribbon1Send->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Ribbon1, RoutingSettings::tAspectIndex::LOCAL, false);
    TestHelper::doMainLoopIteration();

    THEN("Ribbon Parameter is set to return position, Send Parameter is set to oldCP")
    {
      CHECK(ribbon1->getDisplayString() == "100.0 %");
      CHECK(ribbon1Send->getDisplayString() == "100.0 %");
    }
  }
}

//Bender
TEST_CASE("Issue 3035, Bender Local On to Local Off", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();
  TestHelper::initSingleEditBuffer();

  ParameterId srcID = { C15::PID::Bender, VoiceGroup::Global };
  auto bender = eb->findAndCastParameterByID<PitchbendParameter>(srcID);
  auto benderSend
      = eb->findAndCastParameterByID<HardwareSourceSendParameter>({C15::PID::Bender_Send, VoiceGroup::Global});

  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
  settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Bender, RoutingSettings::tAspectIndex::LOCAL, true);
  TestHelper::doMainLoopIteration();

  ParameterUseCases sendUseCase(benderSend);

  sendUseCase.setControlPosition(0.5);
  bender->onChangeFromExternalSource(1, HWChangeSource::TCD);

  CHECK(bender->getDisplayString() == "100.0 %");
  CHECK(benderSend->getDisplayString() == "50.0 %");

  WHEN("Local is set to Off")
  {
    settings->getSetting<GlobalLocalEnableSetting>()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
    settings->getSetting<RoutingSettings>()->setState(RoutingSettings::tRoutingIndex::Bender, RoutingSettings::tAspectIndex::LOCAL, false);
    TestHelper::doMainLoopIteration();

    THEN("Pedal Parameter RVC -> return position, send to oldCP")
    {
      CHECK(bender->getDisplayString() == "0.0 %");
      CHECK(benderSend->getDisplayString() == "100.0 %");
    }
  }
}