#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "testing/TestHelper.h"
#include <presets/Preset.h>

TEST_CASE("Override Single Preset With new!", "[Preset][Store]")
{
  MockPresetStorage presets;
  auto toOverwrite = presets.getSinglePreset();
  auto eb = TestHelper::getEditBuffer();

  TestHelper::initSingleEditBuffer();
  {
    auto scope = TestHelper::createTestScope();
    eb->setName(scope->getTransaction(), "Name");
    toOverwrite->copyFrom(scope->getTransaction(), eb);
    CHECK(toOverwrite->getName() == "Name");
  }
}

TEST_CASE("Override Dual Preset With new!", "[Preset][Store]")
{
  MockPresetStorage presets;
  auto toOverwrite = presets.getLayerPreset();
  auto eb = TestHelper::getEditBuffer();

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
  {
    auto scope = TestHelper::createTestScope();

    eb->setVoiceGroupName(scope->getTransaction(), "1", VoiceGroup::I);
    eb->setVoiceGroupName(scope->getTransaction(), "2", VoiceGroup::II);
    eb->setName(scope->getTransaction(), "Name");

    toOverwrite->copyFrom(scope->getTransaction(), eb);

    CHECK(toOverwrite->getVoiceGroupName(VoiceGroup::I) == "1");
    CHECK(toOverwrite->getVoiceGroupName(VoiceGroup::II) == "2");
    CHECK(toOverwrite->getName() == "Name");
  }
}