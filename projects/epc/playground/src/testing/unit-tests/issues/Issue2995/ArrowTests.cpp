#include <testing/TestHelper.h>
#include <proxies/hwui/descriptive-layouts/EventProvider.h>
#include <proxies/hwui/descriptive-layouts/events/GlobalEventSourceBroker.h>
#include "proxies/hwui/descriptive-layouts/events/event-sources/edit-buffer/EditBufferEvents.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "ArrowTests -> no unassigned states")
{
  auto eb = TestHelper::getEditBuffer();
  const auto toFX_I = eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  const auto toFX_II = eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
  const auto outmixer_lvl_I = eb->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I });
  const auto outmixer_lvl_II = eb->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::II });

  auto scope = TestHelper::createTestScope();

  WHEN("to FX I is set")
  {
    auto cpIToFXII = GENERATE(0, 0.5, 1);
    toFX_I->setCPFromHwui(scope->getTransaction(), cpIToFXII);

    WHEN("to FX II is set")
    {
      auto cpIIToFXI = GENERATE(0, 0.5, 1);
      toFX_II->setCPFromHwui(scope->getTransaction(), cpIIToFXI);

      WHEN("outmix I is set")
      {
        auto outI = GENERATE(0, 1);
        outmixer_lvl_I->setCPFromHwui(scope->getTransaction(), outI);

        WHEN("outmix II is set")
        {
          auto outII = GENERATE(0, 1);
          outmixer_lvl_II->setCPFromHwui(scope->getTransaction(), outII);

          THEN("no unassigned state is left")
          {
            INFO("I fx to II at " << cpIToFXII);
            INFO("II fx to I at " << cpIIToFXI);
            INFO("I outmixer at " << outI);
            INFO("II outmixer at " << outII);

            auto& foo = DescriptiveLayouts::getGlobalEventSourceBroker();
            auto eventSource = foo.getEventSource(DescriptiveLayouts::EventSources::LayerToFXPath);
            auto strEventSource = dynamic_cast<DescriptiveLayouts::LayerToFXPath*>(eventSource);
            strEventSource->onChange(eb);
            CHECK(strEventSource->getValue() != "ERROR");
          }
        }
      }
    }
  }
}