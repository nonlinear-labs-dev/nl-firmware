//
// Created by justus on 08.03.22.
//
#include <testing/TestHelper.h>
#include "parameters/PitchbendParameter.h"
#include "device-settings/GlobalLocalEnableSetting.h"
#include "device-settings/BooleanSetting.h"
#include "mock/MockDSPHosts.h"
#include "MidiRuntimeOptions.h"
#include "synth/input/MidiChannelModeMessages.h"
#include "mock/TCDHelpers.h"
#include "AudioEngineOptions.h"

//Bender
TEST_CASE("Real Synth, Issue 3035, Bender Local On to Local Off")
{
  auto realoptionsPG = std::vector<std::string>{"playground"};
  int argcPG = realoptionsPG.size();
  char ** argvPG = new char*[argcPG];
  int iPG = 0;
  for(auto o: realoptionsPG)
  {
    argvPG[iPG] = new char[o.size()];
    strcpy(argvPG[iPG], o.data());
    iPG++;
  }
  CHECK(strcmp(argvPG[0], "playground") == 0);

  Application app(argcPG, argvPG);


  auto realoptions = std::vector<std::string>{"acceptance-test", "-a", "default", "-n", "128", "-s", "256", "-d", "localhost"};
  int argc = realoptions.size();
  char ** argv = new char*[argc];
  int i = 0;
  for(auto o: realoptions)
  {
    argv[i] = new char[o.size()];
    strcpy(argv[i], o.data());
    i++;
  }
  CHECK(strcmp(argv[0], "acceptance-test") == 0);
  CHECK(strcmp(argv[1], "-a") == 0);

  AudioEngineOptions options(argc, argv);
  C15Synth synth(&options);

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

  CHECK(bender->getDisplayString() == "0.0 %");

  ParameterUseCases sendUseCase(benderSend);
  sendUseCase.setControlPosition(0.5);
  synth.doTcd(TCD_HELPER::createFullPressureHWEvent(TCD_HELPER::TCD_HW_IDS::Bender));
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  TestHelper::doMainLoopIteration();

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

//  nltools::msg::init(oldConfig);
}