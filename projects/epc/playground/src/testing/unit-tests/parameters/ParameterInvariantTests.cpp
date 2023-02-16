#include <groups/HardwareSourcesGroup.h>
#include <groups/MacroControlMappingGroup.h>
#include <groups/MacroControlsGroup.h>
#include <groups/ScaleGroup.h>
#include <groups/MasterGroup.h>
#include <catch.hpp>
#include "testing/TestHelper.h"
#include "presets/EditBuffer.h"
#include "parameters/ParameterFactory.h"

namespace GroupAffiliation
{
  inline bool isRealGlobal(const Parameter* parameter)
  {
    auto parent = parameter->getParent();
    return dynamic_cast<HardwareSourcesGroup*>(parent) || dynamic_cast<MacroControlMappingGroup*>(parent)
        || dynamic_cast<MacroControlsGroup*>(parent) || dynamic_cast<ScaleGroup*>(parent)
        || dynamic_cast<MasterGroup*>(parent);
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Parameter Dual/Global affiliation - Single-Sound")
{
  TestHelper::forEachParameter(
      [](const Parameter* parameter) {
        CHECK_FALSE(EditBuffer::isDualParameterForSoundType(parameter, SoundType::Single));
      },
      TestHelper::getEditBuffer());
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Parameter Dual/Global affiliation - Split Sound")
{
  TestHelper::forEachParameter(
      [](const Parameter* parameter) {
        auto ret = EditBuffer::isDualParameterForSoundType(parameter, SoundType::Split);

        if(GroupAffiliation::isRealGlobal(parameter))
          CHECK_FALSE(ret);
        else
          CHECK(ret);
      },
      TestHelper::getEditBuffer());
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Parameter Dual/Global affiliation - Layer Sound")
{
  TestHelper::forEachParameter(
      [](const Parameter* parameter) {
        auto isDual = EditBuffer::isDualParameterForSoundType(parameter, SoundType::Layer);

        if(GroupAffiliation::isRealGlobal(parameter))
        {
          CHECK_FALSE(isDual);
        }
        else
        {
          if(ParameterFactory::isMonoParameter(parameter))
          {
            CHECK_FALSE(isDual);
          }
          else if(ParameterFactory::isUnisonParameter(parameter))
          {
            CHECK_FALSE(isDual);
          }
          else
          {
            CHECK(isDual);
          }
        }
      },
      TestHelper::getEditBuffer());
}