#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/FuxieSwarmsTestBank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/scale-converters/Linear12CountScaleConverter.h>
#include <parameters/scale-converters/LinearCountScaleConverter.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>

TEST_CASE("Import Bank Fresh Results in Correct Voices", "[Unison]")
{
  FuxieSwarmsTestBank bank;
  auto preset = bank.getPreset(0);
  auto eb = TestHelper::getEditBuffer();

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initSingleEditBuffer(scope->getTransaction());
  }

  WHEN("Unison Voices Correct")
  {
    PresetParameter* unisonVoices = nullptr;
    CHECK_NOTHROW(unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true));
    CHECK(unisonVoices);

    const auto ogValue = unisonVoices->getValue();
    auto voicesI = eb->findParameterByID({ 249, VoiceGroup::I });
    CHECK(voicesI->getValue().getDisplayString(ogValue) == "8 voices");
    CHECK(voicesI->getValue().getDisplayString() == "1 voice (off)");

    THEN("Load Preset -> voices == 8")
    {
      auto scope = TestHelper::createTestScope();
      auto transaction = scope->getTransaction();
      eb->undoableLoad(transaction, preset);

      CHECK(eb->getType() == SoundType::Single);
      CHECK(voicesI->getDisplayString() == "8 voices");
    }

    THEN("Load into Dual -> voices == 8")
    {
      auto scope = TestHelper::createTestScope();
      auto transaction = scope->getTransaction();
      TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
      eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::I);

      CHECK(eb->getType() == SoundType::Layer);
      CHECK(voicesI->getDisplayString() == "8 voices");
    }
  }
}