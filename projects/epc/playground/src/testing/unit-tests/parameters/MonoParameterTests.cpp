#include <testing/TestHelper.h>
#include <catch.hpp>
#include "presets/EditBuffer.h"
#include "parameters/ParameterFactory.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Mono Parameter Helper")
{
  auto eb = TestHelper::getEditBuffer();
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    if(auto g = eb->getParameterGroupByID({ "Mono", vg }))
    {
      for(auto& p : g->getParameters())
      {
        CHECK(ParameterFactory::isMonoParameter(p));
      }
    }
    else
    {
      CHECK(false);
    }
  }
}