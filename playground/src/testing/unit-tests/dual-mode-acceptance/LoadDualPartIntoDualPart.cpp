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

template <SoundType tType, VoiceGroup tLoadFromPart, VoiceGroup tLoadToVoiceGroup>
void LoadDualPresetPartWithValueIntoInitDualSoundPart(Preset *preset, tControlPositionValue paramValue)
{
  //WHEN("converted to " + toString(tType) + " load into " + toString(tLoadToVoiceGroup) + " from "
  //    + toString(tLoadFromPart) + " of type " + toString(preset->getType()))
  //{
  static_assert(tType == SoundType::Split || tType == SoundType::Layer, "");

  constexpr auto globalValue = 0.75;

  auto editBuffer = TestHelper::getEditBuffer();

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initDualEditBuffer<tType>();

    preset->forEachParameter([&](PresetParameter *pp) { pp->setValue(scope->getTransaction(), paramValue); });
    editBuffer->forEachParameter<VoiceGroup::Global>(
        [&](Parameter *p) { p->setCPFromHwui(scope->getTransaction(), globalValue); });

    editBuffer->undoableLoadPresetPartIntoPart(scope->getTransaction(), preset, tLoadFromPart, tLoadToVoiceGroup);
  }

  //THEN(toString(tLoadToVoiceGroup) + " has preset values and GLOBAL was not touched")
  //{
  editBuffer->forEachParameter<tLoadToVoiceGroup>(
      [&](Parameter *p) { CHECK_PARAMETER_CP_EQUALS_FICTION(p, paramValue); });

  editBuffer->forEachParameter<VoiceGroup::Global>(
      [&](Parameter *p) { CHECK_PARAMETER_CP_EQUALS_FICTION(p, globalValue); });
  //}
  //}
}

TEST_CASE("Load Dual Part into Dual Part", "[EditBuffer][Loading]")
{
  MockPresetStorage presets;
  auto layerpreset = presets.getLayerPreset();

  for(auto val : std::vector<double> { 0.5, 0.0, 1.0 })
  {
    WHEN("Test with value: " + std::to_string(val))
    {
      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::I, VoiceGroup::I>(layerpreset,
                                                                                                       val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::I, VoiceGroup::II>(layerpreset,
                                                                                                        val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::II, VoiceGroup::I>(layerpreset,
                                                                                                        val);

      LoadDualPresetPartWithValueIntoInitDualSoundPart<SoundType::Layer, VoiceGroup::II, VoiceGroup::II>(layerpreset,
                                                                                                         val);
    }
  }
}
