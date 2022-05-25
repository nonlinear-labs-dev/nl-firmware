#include <catch.hpp>
#include <parameters/ModulateableParameter.h>
#include <testing/TestHelper.h>
#include <testing/parameter/TestGroup.h>
#include <testing/parameter/TestGroupSet.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <sync/SyncMasterMockRoot.h>
#include "testing/unit-tests/mock/MockSettingsObject.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Modulateable Parameter")
{
  MockSettingsObject settings("/tmp/dksakda.xml", &SyncMasterMockRoot::get());
  TestGroupSet groupSet(&SyncMasterMockRoot::get());
  groupSet.init(&settings);
  TestGroup group(&groupSet, VoiceGroup::Global);

  SECTION("1.4pct to 112 playcontroller")
  {
    auto scope = TestHelper::createTestScope();
    ModulateableParameter peter(&group, { C15::PID::FB_Mix_FX, VoiceGroup::Global },
                                ScaleConverter::get<Linear100PercentScaleConverter>());
    peter.setModulationAmount(scope->getTransaction(), 0.014);
    peter.setModulationSource(scope->getTransaction(), MacroControls::MC1);
    auto packed = peter.getModulationSourceAndAmountPacked();
    packed &= 0x3FFF;
    REQUIRE(packed == 14);
  }
}
