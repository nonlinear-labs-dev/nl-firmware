#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "testing/TestHelper.h"
#include "third-party/include/catch.hpp"
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE("Load Dual Part into Part")
{
  MockPresetStorage presets;
  auto editBuffer = TestHelper::getEditBuffer();

  auto splitPreset = presets.getSplitPreset();
  auto presetTuneI = splitPreset->findParameterByID({ 358, VoiceGroup::I });
  auto presetTuneII = splitPreset->findParameterByID({ 358, VoiceGroup::II });

  //Prepare Presets
  {
    auto scope = TestHelper::createTestScope();

    presetTuneI->setValue(scope->getTransaction(), 0.555);
    presetTuneII->setValue(scope->getTransaction(), 0.000187);
  }

  WHEN("Init Sound is Split")
  {
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
      editBuffer->undoableInitSound(scope->getTransaction());

      REQUIRE(editBuffer->getType() == SoundType::Split);
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }

    THEN("Load Split Part I into I")
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoadPresetPartIntoPart(scope->getTransaction(), splitPreset, VoiceGroup::I, VoiceGroup::I);
      REQUIRE(editBuffer->findParameterByID({ 358, VoiceGroup::I })->getControlPositionValue() == 0.555);
    }

    THEN("Load Split Part I into II")
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoadPresetPartIntoPart(scope->getTransaction(), splitPreset, VoiceGroup::I, VoiceGroup::II);
      REQUIRE(editBuffer->findParameterByID({ 358, VoiceGroup::II })->getControlPositionValue() == 0.555);
    }

    THEN("Load Split Part II into I")
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoadPresetPartIntoPart(scope->getTransaction(), splitPreset, VoiceGroup::II, VoiceGroup::I);
      REQUIRE(editBuffer->findParameterByID({ 358, VoiceGroup::I })->getControlPositionValue() == 0.000187);
    }

    THEN("Load Split Part II into II")
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoadPresetPartIntoPart(scope->getTransaction(), splitPreset, VoiceGroup::II, VoiceGroup::II);
      REQUIRE(editBuffer->findParameterByID({ 358, VoiceGroup::II })->getControlPositionValue() == 0.000187);
    }
  }
}
