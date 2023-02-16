#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "testing/TestHelper.h"
#include <presets/Preset.h>
#include <use-cases/PresetUseCases.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Override Single Preset With new!", "[Preset][Store]")
{
  MockPresetStorage presets;
  auto toOverwrite = presets.getSinglePreset();
  auto eb = TestHelper::getEditBuffer();

  TestHelper::initSingleEditBuffer();
  {
    auto scope = TestHelper::createTestScope();
    eb->setName(scope->getTransaction(), "Name");
  }
  PresetUseCases presetUseCases(*toOverwrite, *TestHelper::getSettings());
  presetUseCases.overwriteWithEditBuffer(*eb);

  CHECK(toOverwrite->getName() == "Name");
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Override Dual Preset With new!", "[Preset][Store]")
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
  }
  PresetUseCases presetUseCase(*toOverwrite, *TestHelper::getSettings());
  presetUseCase.overwriteWithEditBuffer(*eb);

  CHECK(toOverwrite->getVoiceGroupName(VoiceGroup::I) == "1");
  CHECK(toOverwrite->getVoiceGroupName(VoiceGroup::II) == "2");
  CHECK(toOverwrite->getName() == "Name");
}