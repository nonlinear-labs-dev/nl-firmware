#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include <presets/Preset.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "load to part of single sounds")
{
  MockPresetStorage presets;
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  auto single = presets.getSinglePreset();

  WHEN("single preset is loaded into layer")
  {
    auto layer = presets.getLayerPreset();
    ebUseCases.load(layer);
    CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == layer->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);
    CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == layer->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);

    WHEN("I -> I")
    {
      ebUseCases.loadToPart(single, VoiceGroup::I, VoiceGroup::I);
      CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);
    }

    WHEN("I -> II")
    {
      ebUseCases.loadToPart(single, VoiceGroup::I, VoiceGroup::II);
      CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::I);
    }

    WHEN("II -> I")
    {
      ebUseCases.loadToPart(single, VoiceGroup::II, VoiceGroup::I);
      CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::II);
    }

    WHEN("II -> II")
    {
      ebUseCases.loadToPart(single, VoiceGroup::II, VoiceGroup::II);
      CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);
    }
  }
}