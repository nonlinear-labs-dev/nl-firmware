#include "third-party/include/catch.hpp"
#include "testing/TestHelper.h"
#include "MockPresetStorage.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/EditBuffer.h>

TEST_CASE("HWUI Voice Group Selection")
{
  MockPresetStorage presets;
  auto vgSelectionObject = TestHelper::getVoiceGroupSelectionObject();
  auto eb = TestHelper::getEditBuffer();
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
    vgSelectionObject->setHWUIEditBufferSelection(VoiceGroup::II);
    REQUIRE(vgSelectionObject->getEditBufferSelection() == VoiceGroup::II);
  }

  SECTION("Reset Voice Group Selection to I on Preset Load")
  {
    auto scope = TestHelper::createTestScope();
    REQUIRE(vgSelectionObject->getEditBufferSelection() == VoiceGroup::II);
    eb->undoableLoad(scope->getTransaction(), presets.getSplitPreset());
    REQUIRE(vgSelectionObject->getEditBufferSelection() == VoiceGroup::I);
  }
}