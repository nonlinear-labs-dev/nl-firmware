#include <catch.hpp>
#include "testing/TestHelper.h"
#include "mock/MockPresetStorage.h"
#include "parameters/PedalParameter.h"
#include "use-cases/SettingsUseCases.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <groups/HardwareSourcesGroup.h>
#include <parameters/RibbonParameter.h>
#include <parameters/ModulationRoutingParameter.h>

SCENARIO("Ribbon Return Mode")
{
  auto eb = TestHelper::getEditBuffer();
  auto ribbon
      = dynamic_cast<RibbonParameter*>(eb->findParameterByID(HardwareSourcesGroup::getUpperRibbonParameterID()));

  REQUIRE(ribbon);

  WHEN("Set Return Mode to center")
  {
    {
      auto scope = TestHelper::createTestScope();
      ribbon->undoableSetRibbonReturnMode(scope->getTransaction(), RibbonReturnMode::RETURN);
    }

    CHECK(ribbon->getRibbonReturnMode() == RibbonReturnMode::RETURN);

    THEN("Route to A and B")
    {
      auto rToA = dynamic_cast<ModulationRoutingParameter*>(eb->findParameterByID({ 285, VoiceGroup::Global }));
      auto rToB = dynamic_cast<ModulationRoutingParameter*>(eb->findParameterByID({ 286, VoiceGroup::Global }));

      {
        auto scope = TestHelper::createTestScope();
        rToA->setCPFromHwui(scope->getTransaction(), 0.1);
        rToB->setCPFromHwui(scope->getTransaction(), 0.2);
      }

      CHECK_FALSE(rToA->getValue().isBoolean());
      CHECK_FALSE(rToB->getValue().isBoolean());

      WHEN("Set Return Mode to None")
      {
        {
          auto scope = TestHelper::createTestScope();
          ribbon->undoableSetRibbonReturnMode(scope->getTransaction(), RibbonReturnMode::STAY);
        }

        CHECK(rToA->getValue().isBoolean());
        CHECK(rToB->getValue().isBoolean());

        CHECK(rToA->getValue().getRawValue() == 0.0);  //Gets reset to 0
        CHECK(rToB->getValue().getRawValue() == 0.2);  //stays at 0.2 but appears bidirectional as it is boolean value
      }
    }
  }
}

SCENARIO("Local Enable - Disable BiDi Modulation")
{
  auto eb = TestHelper::getEditBuffer();

  auto ribbon = dynamic_cast<RibbonParameter*>(eb->findParameterByID(HardwareSourcesGroup::getUpperRibbonParameterID()));
  auto pedal1 = dynamic_cast<PedalParameter*>(eb->findParameterByID(HardwareSourcesGroup::getPedal1ParameterID()));
  auto ribbonA = eb->findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global});
  auto pedalA = eb->findParameterByID({C15::PID::Pedal_1_to_MC_A, VoiceGroup::Global});
  auto mcA = eb->findParameterByID({C15::PID::MC_A, VoiceGroup::Global});

  REQUIRE(ribbon);
  REQUIRE(pedal1);
  REQUIRE(ribbonA);
  REQUIRE(pedalA);
  REQUIRE(mcA);

  WHEN("Set Return Mode to None and Pedal to Direct and Ribbon to Bi-Di")
  {
    {
      auto scope = TestHelper::createTestScope();
      auto transaction = scope->getTransaction();
      ribbon->undoableSetRibbonReturnMode(transaction, RibbonReturnMode::STAY);
      pedal1->undoableSetPedalMode(transaction, PedalModes::STAY, Initiator::EXPLICIT_USECASE);
      pedalA->getValue().setIsBoolean(true);
      ribbonA->getValue().setIsBoolean(true);
      pedalA->setCPFromHwui(transaction, 1);
      ribbonA->setCPFromHwui(transaction, 1);

      CHECK(ribbonA->getValue().isBoolean());
      CHECK(pedalA->getValue().isBoolean());
    }

    SettingsUseCases useCases(*TestHelper::getSettings());
    useCases.setAllRoutingEntries(true);

    auto to = [](auto x) { return static_cast<int>(x); };
    auto initPos = [&](){
      auto scope = TestHelper::createTestScope();
      auto trans = scope->getTransaction();
      ribbon->setCPFromHwui(trans, 0);
      pedal1->setCPFromHwui(trans, 0);
      mcA->setCPFromHwui(trans, 0);

      CHECK(mcA->getControlPositionValue() == 0);
      CHECK(ribbon->getControlPositionValue() == 0);
      CHECK(pedal1->getControlPositionValue() == 0);
    };

    WHEN("Ribbon 1 Local enabled")
    {
      useCases.updateRoutingAspect(to(RoutingSettings::tRoutingIndex::Ribbon1), to(RoutingSettings::tAspectIndex::LOCAL), true);
      initPos();

      WHEN("Pedal1 is set to 0.5")
      {
        ParameterUseCases p1UseCases(pedal1);
        p1UseCases.setControlPosition(0.5);

        THEN("Ribbon 1 is 0.5")
        {
          CHECK(ribbon->getControlPositionValue() == 0.5);
        }

        THEN("MC A is 0.5")
        {
          CHECK(mcA->getControlPositionValue() == 0.5);
        }
      }
    }

    WHEN("Ribbon 1 Local disabled")
    {
      useCases.updateRoutingAspect(to(RoutingSettings::tRoutingIndex::Ribbon1), to(RoutingSettings::tAspectIndex::LOCAL), false);
      initPos();

      WHEN("Pedal1 is set to 0.5")
      {
        ParameterUseCases p1UseCases(pedal1);
        p1UseCases.setControlPosition(0.5);

        THEN("Ribbon 1 is 0.5")
        {
          CHECK(ribbon->getControlPositionValue() == 0.5);
        }

        THEN("MC A is 0.5")
        {
          CHECK(mcA->getControlPositionValue() == 0.5);
        }
      }
    }
  }
}