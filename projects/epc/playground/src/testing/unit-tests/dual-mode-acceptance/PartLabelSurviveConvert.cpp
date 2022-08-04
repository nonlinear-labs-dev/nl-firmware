#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <presets/Preset.h>
#include <presets/PresetManager.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Convert Split to Layer With Part Names")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);

  auto setVGName = [&](auto transaction, auto vg, auto name) { eb->setVoiceGroupName(transaction, name, vg); };

  {
    THEN("Split -> Layer")
    {
      TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);

      {
        auto scope = TestHelper::createTestScope();
        auto trans = scope->getTransaction();
        setVGName(trans, VoiceGroup::I, "I");
        setVGName(trans, VoiceGroup::II, "II");
      }

      useCase.convertToLayer(VoiceGroup::I);

      CHECK(eb->getVoiceGroupName(VoiceGroup::I) == "I");
      CHECK(eb->getVoiceGroupName(VoiceGroup::II) == "II");
    }

    THEN("Layer -> Split")
    {
      TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

      {
        auto scope = TestHelper::createTestScope();
        auto trans = scope->getTransaction();
        setVGName(trans, VoiceGroup::I, "I");
        setVGName(trans, VoiceGroup::II, "II");
      }

      useCase.convertToSplit(VoiceGroup::I);

      CHECK(eb->getVoiceGroupName(VoiceGroup::I) == "I");
      CHECK(eb->getVoiceGroupName(VoiceGroup::II) == "II");
    }
  }
}