#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/EditBufferNamedLogicalParts.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <parameter_declarations.h>

TEST_CASE("Store and Load Preset yields same editbuffer")
{
  MockPresetStorage presets;

  auto layerPreset = presets.getLayerPreset();

  using EBL = EditBufferLogicalParts;
  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);

  WHEN("Unison Voices Changed")
  {
    useCase.setParameter({C15::PID::Unison_Voices, VoiceGroup::I}, 1);

    THEN("Stored")
    {
      {
        auto scope = TestHelper::createTestScope();
        layerPreset->copyFrom(scope->getTransaction(), eb);
      }

      WHEN("Loaded")
      {
        useCase.load(layerPreset);

        THEN("Unison Voices are 12")
        {
          auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
          CHECK(voicesI->getDisplayString() == "12 voices");
        }
      }
    }
  }
}