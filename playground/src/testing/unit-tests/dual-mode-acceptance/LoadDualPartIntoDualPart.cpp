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

template <SoundType tType, VoiceGroup tLoadFromPart, VoiceGroup tLoadToVoiceGroup>
void LoadDualPresetPartWithValueIntoInitDualSoundPart(Preset *preset, tControlPositionValue paramValue)
{
  WHEN("converted to " + toString(tType) + " load into " + toString(tLoadToVoiceGroup) + " from "
       + toString(tLoadFromPart) + "of type " + toString(preset->getType()))
  {
    static_assert(tType == SoundType::Split || tType == SoundType::Layer);

    auto editBuffer = TestHelper::getEditBuffer();

    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), tType);
      editBuffer->undoableInitSound(scope->getTransaction());

      preset->forEachParameter([&](PresetParameter *pp) { pp->setValue(scope->getTransaction(), paramValue); });

      editBuffer->undoableLoadPresetPartIntoPart(scope->getTransaction(), preset, tLoadFromPart, tLoadToVoiceGroup);
    }

    THEN(toString(tLoadToVoiceGroup) + " has preset values")
    {
      editBuffer->forEachParameter(
          tLoadToVoiceGroup, [&](Parameter *p) { REQUIRE(isValueCloseEnoughToExpectedToMakeSense(p, paramValue)); });
    }
  }
}

TEST_CASE("Load Dual Part into Dual Part")
{
  MockPresetStorage presets;
  auto layerpreset = presets.getLayerPreset();
  auto splitpreset = presets.getSplitPreset();

  for(auto val : std::vector<double>{ 0.187, 0.845, 0.34, 0, 1, 0.94 })
  {
    WHEN("Test with value: " + std::to_string(val))
    {
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::I, VoiceGroup::I>(layerpreset,
                                                                                                       val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::I, VoiceGroup::I>(layerpreset,
                                                                                                       val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::I, VoiceGroup::I>(splitpreset,
                                                                                                       val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::I, VoiceGroup::I>(splitpreset,
                                                                                                       val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::I, VoiceGroup::II>(layerpreset,
                                                                                                        val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::I, VoiceGroup::II>(layerpreset,
                                                                                                        val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::I, VoiceGroup::II>(splitpreset,
                                                                                                        val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::I, VoiceGroup::II>(splitpreset,
                                                                                                        val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::II, VoiceGroup::I>(layerpreset,
                                                                                                        val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::II, VoiceGroup::I>(layerpreset,
                                                                                                        val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::II, VoiceGroup::I>(splitpreset,
                                                                                                        val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::II, VoiceGroup::I>(splitpreset,
                                                                                                        val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::II, VoiceGroup::II>(layerpreset,
                                                                                                         val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::II, VoiceGroup::II>(layerpreset,
                                                                                                         val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::II, VoiceGroup::II>(splitpreset,
                                                                                                         val);
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Split, VoiceGroup::II, VoiceGroup::II>(splitpreset,
                                                                                                         val);
    }
  }
}