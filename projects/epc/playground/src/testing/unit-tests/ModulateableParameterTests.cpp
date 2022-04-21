#include <catch.hpp>
#include <parameters/ModulateableParameter.h>
#include <testing/TestHelper.h>
#include <testing/parameter/TestGroup.h>
#include <testing/parameter/TestGroupSet.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE("Modulateable Parameter")
{
  TestGroupSet groupSet(&SyncMasterMockRoot::get());
  groupSet.init(nullptr);
  TestGroup group(&groupSet, VoiceGroup::I);

  SECTION("1.4pct to 112 playcontroller")
  {
    auto scope = TestHelper::createTestScope();
    ModulateableParameter peter(&group, { 1, VoiceGroup::Global },
                                ScaleConverter::get<Linear100PercentScaleConverter>());
    peter.setModulationAmount(scope->getTransaction(), 0.014);
    peter.setModulationSource(scope->getTransaction(), MacroControls::MC1);
    auto packed = peter.getModulationSourceAndAmountPacked();
    packed &= 0x3FFF;
    REQUIRE(packed == 14);
  }
}
