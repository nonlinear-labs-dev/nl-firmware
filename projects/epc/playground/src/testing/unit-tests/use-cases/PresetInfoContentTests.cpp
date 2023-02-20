#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <presets/PresetParameter.h>
#include <presets/Preset.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetInfoContent.h>

namespace detail
{
  template <VoiceGroup vg> void enableMono(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto mono = preset->findParameterByID({ C15::PID::Mono_Grp_Enable, vg }, true);
    mono->setValue(scope->getTransaction(), 1);
    preset->setAutoGeneratedAttributes(scope->getTransaction());
  }

  template <VoiceGroup vg> void disableMono(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto mono = preset->findParameterByID({ C15::PID::Mono_Grp_Enable, vg }, true);
    mono->setValue(scope->getTransaction(), 0);
    preset->setAutoGeneratedAttributes(scope->getTransaction());
  }

  template <VoiceGroup vg> void enableUnison(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto unison = preset->findParameterByID({ C15::PID::Unison_Voices, vg }, true);
    unison->setValue(scope->getTransaction(), 1);
    preset->setAutoGeneratedAttributes(scope->getTransaction());
  }

  template <VoiceGroup vg> void disableUnison(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto unison = preset->findParameterByID({ C15::PID::Unison_Voices, vg }, true);
    unison->setValue(scope->getTransaction(), 0);
    preset->setAutoGeneratedAttributes(scope->getTransaction());
  }
};

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Single Preset Type String", "[Preset]")
{
  MockPresetStorage presets;
  auto preset = presets.getSinglePreset();
  detail::disableMono<VoiceGroup::I>(preset);
  detail::disableMono<VoiceGroup::II>(preset);
  detail::disableUnison<VoiceGroup::I>(preset);
  detail::disableUnison<VoiceGroup::II>(preset);

  SECTION("Mono Enabled")
  {
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Single (Mono)");
    CHECK(preset->getHashtags() == "#Single, #Mono");
  }

  SECTION("Unison Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Single (Unison)");
    CHECK(preset->getHashtags() == "#Single, #Unison");
  }

  SECTION("Unison and Mono Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Single (Mono, Unison)");
    CHECK(preset->getHashtags() == "#Single, #Mono, #Unison");
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Layer Preset Type String", "[Preset]")
{
  MockPresetStorage presets;
  auto preset = presets.getLayerPreset();

  detail::disableMono<VoiceGroup::I>(preset);
  detail::disableMono<VoiceGroup::II>(preset);
  detail::disableUnison<VoiceGroup::I>(preset);
  detail::disableUnison<VoiceGroup::II>(preset);

  SECTION("Mono Enabled")
  {
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Layer (Mono)");
    CHECK(preset->getHashtags() == "#Layer, #Mono");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Layer (Unison)");
    CHECK(preset->getHashtags() == "#Layer, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison and Mono Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Layer (Mono, Unison)");
    CHECK(preset->getHashtags() == "#Layer, #Mono, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Split Preset Type String", "[Preset]")
{
  MockPresetStorage presets;
  auto preset = presets.getSplitPreset();

  detail::disableMono<VoiceGroup::I>(preset);
  detail::disableMono<VoiceGroup::II>(preset);
  detail::disableUnison<VoiceGroup::I>(preset);
  detail::disableUnison<VoiceGroup::II>(preset);

  SECTION("None")
  {
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Mono I Enabled")
  {
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono I)");
    CHECK(preset->getHashtags() == "#Split, #Mono");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Mono II Enabled")
  {
    detail::enableMono<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono II)");
    CHECK(preset->getHashtags() == "#Split, #Mono");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Mono I/II Enabled")
  {
    detail::enableMono<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono I/II)");
    CHECK(preset->getHashtags() == "#Split, #Mono");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison I Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Unison I)");
    CHECK(preset->getHashtags() == "#Split, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison II Enabled")
  {
    detail::enableUnison<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Unison II)");
    CHECK(preset->getHashtags() == "#Split, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison I/II Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableUnison<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Unison I/II)");
    CHECK(preset->getHashtags() == "#Split, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison II and Mono I Enabled")
  {
    detail::enableUnison<VoiceGroup::II>(preset);
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono I, Unison II)");
    CHECK(preset->getHashtags() == "#Split, #Mono, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }

  SECTION("Unison I/II and Mono II Enabled")
  {
    detail::enableUnison<VoiceGroup::II>(preset);
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono II, Unison I/II)");
    CHECK(preset->getHashtags() == "#Split, #Mono, #Unison");
    CHECK(preset->getHashtags() == preset->getAttribute("Hashtags", ""));
  }
}
