#include <testing/TestHelper.h>
#include <parameters/RibbonParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameter_declarations.h>
#include <use-cases/RibbonParameterUseCases.h>

TEST_CASE("Setting Ribbon to Return to center leads to HW-Amt to be 100% if boolean was on")
{
  TestHelper::initSingleEditBuffer();

  auto eb = TestHelper::getEditBuffer();
  auto ribbonParam = eb->findAndCastParameterByID<RibbonParameter>({ C15::PID::Ribbon_1, VoiceGroup::Global });
  auto router
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global });

  RibbonParameterUseCases useCases(ribbonParam);
  ParameterUseCases routerUseCases(router);

  WHEN("Ribbon is None-Returning and router is enabled")
  {
    useCases.setReturnMode(RibbonReturnMode::STAY);
    routerUseCases.setControlPosition(1);

    CHECK(router->getValue().isBoolean());
    CHECK(router->getValue().getRawValue() == 1);

    WHEN("Ribbon is changed to RTC")
    {
      useCases.setReturnMode(RibbonReturnMode::RETURN);

      THEN("HW-Amt is 50% and Mode is correct")
      {
        CHECK_FALSE(router->getValue().isBoolean());
        CHECK(router->getValue().getRawValue() == 1);
        CHECK(ribbonParam->getRibbonReturnMode() == RibbonReturnMode::RETURN);

        WHEN("UNDO")
        {
          auto& undoScope = ribbonParam->getUndoScope();
          undoScope.undo();

          THEN("HW-Amt is back at 100% router is Boolean and RetMode is None")
          {
            CHECK(router->getValue().isBoolean());
            CHECK(router->getValue().getRawValue() == 1);
            CHECK(ribbonParam->getRibbonReturnMode() == RibbonReturnMode::STAY);

            WHEN("Redo")
            {
              undoScope.redo();

              CHECK_FALSE(router->getValue().isBoolean());
              CHECK(router->getValue().getRawValue() == 1);
              CHECK(ribbonParam->getRibbonReturnMode() == RibbonReturnMode::RETURN);
            }
          }
        }
      }
    }
  }
}

TEST_CASE("Setting Ribbon to Return to center leads to unchanged HW-Amt if boolean was off")
{
  TestHelper::initSingleEditBuffer();

  auto eb = TestHelper::getEditBuffer();
  auto ribbonParam = eb->findAndCastParameterByID<RibbonParameter>({ C15::PID::Ribbon_1, VoiceGroup::Global });
  auto router
  = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global });

  RibbonParameterUseCases useCases(ribbonParam);
  ParameterUseCases routerUseCases(router);

  WHEN("Ribbon is None-Returning and router is disabled")
  {
    useCases.setReturnMode(RibbonReturnMode::STAY);
    routerUseCases.setControlPosition(0);

    CHECK(router->getValue().isBoolean());
    CHECK(router->getValue().getRawValue() == 0);

    WHEN("Ribbon is changed to RTC")
    {
      useCases.setReturnMode(RibbonReturnMode::RETURN);

      THEN("HW-Amt is still 0% and Mode is correct")
      {
        CHECK_FALSE(router->getValue().isBoolean());
        CHECK(router->getValue().getRawValue() == 0);
        CHECK(ribbonParam->getRibbonReturnMode() == RibbonReturnMode::RETURN);

        WHEN("UNDO")
        {
          auto& undoScope = ribbonParam->getUndoScope();
          undoScope.undo();

          THEN("HW-Amt is still at 0% router is Boolean and RetMode is None")
          {
            CHECK(router->getValue().isBoolean());
            CHECK(router->getValue().getRawValue() == 0);
            CHECK(ribbonParam->getRibbonReturnMode() == RibbonReturnMode::STAY);

            WHEN("Redo")
            {
              undoScope.redo();

              CHECK_FALSE(router->getValue().isBoolean());
              CHECK(router->getValue().getRawValue() == 0);
              CHECK(ribbonParam->getRibbonReturnMode() == RibbonReturnMode::RETURN);
            }
          }
        }
      }
    }
  }
}