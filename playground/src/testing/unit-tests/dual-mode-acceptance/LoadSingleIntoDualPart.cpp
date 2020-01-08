#include <third-party/include/catch.hpp>
#include <testing/TestHelper.h>

#include <presets/EditBuffer.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <nltools/Types.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>

inline bool isValueCloseEnoughToExpectedToMakeSense(const Parameter *p, double expected)
{
  auto denom = p->getValue().getCoarseDenominator();
  auto step = 1.0 / denom;
  return std::abs(p->getControlPositionValue() - expected) <= step;
}

template <SoundType tType, VoiceGroup tLoadToVoiceGroup>
void LoadSinglePresetWithValueIntoInitDualSound(Preset *preset, tControlPositionValue paramValue)
{
  WHEN("converted to " + toString(tType) + " (" + toString(tLoadToVoiceGroup) + ")")
  {

    static_assert(tType == SoundType::Split || tType == SoundType::Layer);

    auto editBuffer = TestHelper::getEditBuffer();

    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), tType);
      editBuffer->undoableInitSound(scope->getTransaction());

      preset->forEachParameter([&](PresetParameter *pp) { pp->setValue(scope->getTransaction(), paramValue); });

      editBuffer->undoableLoadPresetIntoDualSound(scope->getTransaction(), preset, tLoadToVoiceGroup);
    }

    THEN(toString(tLoadToVoiceGroup) + " has preset values")
    {
      editBuffer->forEachParameter(
          tLoadToVoiceGroup, [&](Parameter *p) { REQUIRE(isValueCloseEnoughToExpectedToMakeSense(p, paramValue)); });
    }
  }
}

TEST_CASE("Load Single into Dual Part")
{
  MockPresetStorage presets;
  auto preset = presets.getSinglePreset();

  for(auto val : std::vector<double>{ 0.187, 0.845, 0.34, 0, 1, 0.94 })
  {
    WHEN("Test with value: " + std::to_string(val))
    {
      LoadSinglePresetWithValueIntoInitDualSound<SoundType::Layer, VoiceGroup::I>(preset, val);
      LoadSinglePresetWithValueIntoInitDualSound<SoundType::Split, VoiceGroup::I>(preset, val);
      LoadSinglePresetWithValueIntoInitDualSound<SoundType::Layer, VoiceGroup::II>(preset, val);
      LoadSinglePresetWithValueIntoInitDualSound<SoundType::Split, VoiceGroup::II>(preset, val);
    }
  }
}