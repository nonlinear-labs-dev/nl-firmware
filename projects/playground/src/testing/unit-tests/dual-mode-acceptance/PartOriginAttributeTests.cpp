#include <testing/TestHelper.h>
#include <Application.h>
#include <presets/Preset.h>
#include <device-settings/Settings.h>
#include <device-settings/LoadToPartSetting.h>
#include <device-settings/DirectLoadSetting.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>

namespace detail
{
  auto getLoadToPart()
  {
    return Application::get().getSettings()->getSetting<LoadToPartSetting>();
  }

  auto getDirectLoad()
  {
    return Application::get().getSettings()->getSetting<DirectLoadSetting>();
  }
}

TEST_CASE("Part Origin Attribute")
{
  auto eb = TestHelper::getEditBuffer();

  MockPresetStorage presets;

  auto dlScope = detail::getDirectLoad()->scopedOverlay(BooleanSettings::BOOLEAN_SETTING_FALSE);
  auto lpScope = detail::getLoadToPart()->scopedOverlay(BooleanSettings::BOOLEAN_SETTING_FALSE);

  SECTION("Load Single Full")
  {
    eb->undoableLoad(presets.getSinglePreset());

    auto originI = eb->getPartOrigin(VoiceGroup::I);
    auto originII = eb->getPartOrigin(VoiceGroup::II);

    CHECK(originI.presetUUID == presets.getSinglePreset()->getUuid());
    CHECK(originI.sourceGroup == VoiceGroup::I);
    CHECK(originII.presetUUID == presets.getSinglePreset()->getUuid());
    CHECK(originII.sourceGroup == VoiceGroup::I);
  }

  SECTION("Init Sound Resets")
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getSinglePreset());

    eb->undoableInitSound(scope->getTransaction());
    auto originI = eb->getPartOrigin(VoiceGroup::I);
    auto originII = eb->getPartOrigin(VoiceGroup::II);

    CHECK(originI.presetUUID == "");
    CHECK(originII.presetUUID == "");
    CHECK(originI.sourceGroup == VoiceGroup::Global);
    CHECK(originII.sourceGroup == VoiceGroup::Global);
  }

  SECTION("Init Part Resets")
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getSinglePreset());

    eb->undoableInitPart(scope->getTransaction(), VoiceGroup::I);
    auto originI = eb->getPartOrigin(VoiceGroup::I);
    CHECK(originI.presetUUID == "");
    CHECK(originI.sourceGroup == VoiceGroup::Global);

    eb->undoableInitPart(scope->getTransaction(), VoiceGroup::II);
    auto originII = eb->getPartOrigin(VoiceGroup::II);
    CHECK(originII.presetUUID == "");
    CHECK(originII.sourceGroup == VoiceGroup::Global);
  }

  SECTION("Load Dual Full")
  {
    eb->undoableLoad(presets.getLayerPreset());
    auto originI = eb->getPartOrigin(VoiceGroup::I);
    auto originII = eb->getPartOrigin(VoiceGroup::II);

    CHECK(originI.presetUUID == presets.getLayerPreset()->getUuid());
    CHECK(originII.presetUUID == presets.getLayerPreset()->getUuid());
    CHECK(originI.sourceGroup == VoiceGroup::I);
    CHECK(originII.sourceGroup == VoiceGroup::II);
  }

  SECTION("Load Part Of Dual")
  {
    TestHelper::initDualEditBuffer<SoundType::Layer>();
    auto scope = TestHelper::createTestScope();

    eb->undoableLoadPresetPartIntoPart(scope->getTransaction(), presets.getSplitPreset(), VoiceGroup::I, VoiceGroup::I);

    auto originI = eb->getPartOrigin(VoiceGroup::I);
    CHECK(originI.presetUUID == presets.getSplitPreset()->getUuid());
    CHECK(originI.sourceGroup == VoiceGroup::I);

    eb->undoableLoadPresetPartIntoPart(scope->getTransaction(), presets.getSplitPreset(), VoiceGroup::II,
                                       VoiceGroup::II);

    auto originII = eb->getPartOrigin(VoiceGroup::II);
    CHECK(originII.presetUUID == presets.getSplitPreset()->getUuid());
    CHECK(originII.sourceGroup == VoiceGroup::II);
  }
}
