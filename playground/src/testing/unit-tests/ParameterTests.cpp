#include "../TestHelper.h"
#include "third-party/include/catch.hpp"
#include "presets/EditBuffer.h"

namespace parameterdetail
{
  template <typename tCB> void forAllParametersInAllVoiceGroupsDo(const tCB &cb)
  {
    auto editBuffer = TestHelper::getEditBuffer();

    for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    {
      for(auto &g : editBuffer->getParameterGroups(vg))
      {
        for(auto &p : g->getParameters())
        {
          cb(p);
        }
      }
    }
  }
}

TEST_CASE("Parameters do not Crash")
{
  SECTION("Select Parameter")
  {
    auto editBuffer = TestHelper::getEditBuffer();
    parameterdetail::forAllParametersInAllVoiceGroupsDo([&](const Parameter *p) {
      auto scope = TestHelper::createTestScope();
      REQUIRE_NOTHROW(editBuffer->undoableSelectParameter(scope->getTransaction(), p->getID()));
      REQUIRE(editBuffer->getSelected() == p);
    });
  }
}

TEST_CASE("Parameter Groups are conforming to GroupID rules")
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto &group : TestHelper::getEditBuffer()->getParameterGroups(vg))
    {
      REQUIRE_FALSE(GroupId::isGlobal(group->getID().getName()));
    }
  }

  for(auto &group : TestHelper::getEditBuffer()->getParameterGroups(VoiceGroup::Global))
  {
    REQUIRE(GroupId::isGlobal(group->getID().getName()));
  }
}