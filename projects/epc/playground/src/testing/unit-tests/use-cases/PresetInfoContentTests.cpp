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
  }

  template <VoiceGroup vg> void disableMono(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto mono = preset->findParameterByID({ C15::PID::Mono_Grp_Enable, vg }, true);
    mono->setValue(scope->getTransaction(), 0);
  }

  template <VoiceGroup vg> void enableUnison(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto unison = preset->findParameterByID({ C15::PID::Unison_Voices, vg }, true);
    unison->setValue(scope->getTransaction(), 1);
  }

  template <VoiceGroup vg> void disableUnison(Preset* preset)
  {
    auto scope = TestHelper::createTestScope();
    auto unison = preset->findParameterByID({ C15::PID::Unison_Voices, vg }, true);
    unison->setValue(scope->getTransaction(), 0);
  }
};

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Single Preset Type String", "[Preset]")
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
  }

  SECTION("Unison Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Single (Unison)");
  }

  SECTION("Unison and Mono Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Single (Mono, Unison)");
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Layer Preset Type String", "[Preset]")
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
  }

  SECTION("Unison Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Layer (Unison)");
  }

  SECTION("Unison and Mono Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Layer (Mono, Unison)");
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Split Preset Type String", "[Preset]")
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
  }

  SECTION("Mono I Enabled")
  {
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono I)");
  }

  SECTION("Mono II Enabled")
  {
    detail::enableMono<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono II)");
  }

  SECTION("Mono I/II Enabled")
  {
    detail::enableMono<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono I/II)");
  }

  SECTION("Unison I Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Unison I)");
  }

  SECTION("Unison II Enabled")
  {
    detail::enableUnison<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Unison II)");
  }

  SECTION("Unison I/II Enabled")
  {
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableUnison<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Unison I/II)");
  }

  SECTION("Unison II and Mono I Enabled")
  {
    detail::enableUnison<VoiceGroup::II>(preset);
    detail::enableMono<VoiceGroup::I>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono I, Unison II)");
  }

  SECTION("Unison I/II and Mono II Enabled")
  {
    detail::enableUnison<VoiceGroup::II>(preset);
    detail::enableUnison<VoiceGroup::I>(preset);
    detail::enableMono<VoiceGroup::II>(preset);
    CHECK(PresetInfoContent::createPresetTypeString(preset) == "Split (Mono II, Unison I/II)");
  }
}
