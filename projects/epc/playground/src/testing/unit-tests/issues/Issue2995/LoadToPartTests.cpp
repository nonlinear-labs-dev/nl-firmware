#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include "use-cases/PresetUseCases.h"
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "load to part of single sounds")
{
  MockPresetStorage presets;
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  auto single = presets.getSinglePreset();

  auto polyI = eb->findParameterByID({ C15::PID::Env_A_BP, VoiceGroup::I });
  auto polyII = eb->findParameterByID({ C15::PID::Env_A_BP, VoiceGroup::II });
  auto monophonicI = eb->findParameterByID({ C15::PID::Reverb_Chorus, VoiceGroup::I });
  auto monophonicII = eb->findParameterByID({ C15::PID::Reverb_Chorus, VoiceGroup::II });

  {
    ParameterUseCases puI(polyI);
    ParameterUseCases puII(polyII);
    ParameterUseCases muI(monophonicI);
    ParameterUseCases muII(monophonicII);

    ebUseCases.load(single);

    puI.setControlPosition(0.75);
    puII.setControlPosition(0.31);

    muI.setControlPosition(0.75);
    muII.setControlPosition(0.31);

    PresetUseCases puc(*single, eb->getSettings());
    puc.overwriteWithEditBuffer(*eb);

    CHECK(single->findParameterByID({ C15::PID::Env_A_BP, VoiceGroup::I }, true)->getValue() == Approx(0.75));
    CHECK(single->findParameterByID({ C15::PID::Env_A_BP, VoiceGroup::II }, true)->getValue() == Approx(0.31));
    CHECK(single->findParameterByID({ C15::PID::Reverb_Chorus, VoiceGroup::I }, true)->getValue() == Approx(0.75));
    CHECK(single->findParameterByID({ C15::PID::Reverb_Chorus, VoiceGroup::II }, true)->getValue() == Approx(0.31));
  }

  WHEN("single preset is loaded into layer")
  {
    auto layer = presets.getLayerPreset();

    ebUseCases.load(layer);
    CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == layer->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);
    CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == layer->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);

    CHECK(polyI->getControlPositionValue() != Approx(0.75));
    CHECK(polyII->getControlPositionValue() != Approx(0.31));
    CHECK(monophonicI->getControlPositionValue() != Approx(0.75));
    CHECK(monophonicII->getControlPositionValue() != Approx(0.31));

    WHEN("I -> I")
    {
      auto monoBeforeLoad = monophonicII->getControlPositionValue();
      auto polyBeforeLoad = polyII->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::I, VoiceGroup::I);
      CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);

      CHECK(monophonicI->getControlPositionValue() == Approx(0.75));
      CHECK(monophonicII->getControlPositionValue() == monoBeforeLoad);
      CHECK(polyI->getControlPositionValue() == Approx(0.75));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);
    }

    WHEN("I -> II")
    {
      auto polyBeforeLoad = polyI->getControlPositionValue();
      auto monoBeforeLoad = monophonicI->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::I, VoiceGroup::II);
      CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::I);

      CHECK(monophonicI->getControlPositionValue() == monoBeforeLoad);
      CHECK(monophonicII->getControlPositionValue() == Approx(0.75));
      CHECK(polyI->getControlPositionValue() == polyBeforeLoad);
      CHECK(polyII->getControlPositionValue() == Approx(0.75));
    }

    WHEN("II -> I")
    {
      auto polyBeforeLoad = polyII->getControlPositionValue();
      auto monoBeforeLoad = monophonicII->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::II, VoiceGroup::I);
      CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::II);

      CHECK(monophonicI->getControlPositionValue() == Approx(0.31));
      CHECK(monophonicII->getControlPositionValue() == monoBeforeLoad);
      CHECK(polyI->getControlPositionValue() == Approx(0.31));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);
    }

    WHEN("II -> II")
    {
      auto monoBeforeLoad = monophonicI->getControlPositionValue();
      auto polyBeforeLoad = polyI->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::II, VoiceGroup::II);
      CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);

      CHECK(monophonicI->getControlPositionValue() == monoBeforeLoad);
      CHECK(monophonicII->getControlPositionValue() == Approx(0.31));
      CHECK(polyI->getControlPositionValue() == polyBeforeLoad);
      CHECK(polyII->getControlPositionValue() == Approx(0.31));
    }
  }

  WHEN("single preset is loaded into split")
  {
    auto split = presets.getLayerPreset();
    ebUseCases.load(split);
    CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == split->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);
    CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == split->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);

    CHECK(polyI->getControlPositionValue() != Approx(0.75));
    CHECK(polyII->getControlPositionValue() != Approx(0.31));
    CHECK(monophonicI->getControlPositionValue() != Approx(0.75));
    CHECK(monophonicII->getControlPositionValue() != Approx(0.31));

    WHEN("I -> I")
    {
      auto monoBeforeLoad = monophonicII->getControlPositionValue();
      auto polyBeforeLoad = polyII->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::I, VoiceGroup::I);
      CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);

      CHECK(monophonicI->getControlPositionValue() == Approx(0.75));
      CHECK(monophonicII->getControlPositionValue() == monoBeforeLoad);
      CHECK(polyI->getControlPositionValue() == Approx(0.75));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);
    }

    WHEN("I -> II")
    {
      auto polyBeforeLoad = polyI->getControlPositionValue();
      auto monoBeforeLoad = monophonicI->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::I, VoiceGroup::II);
      CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::I);

      CHECK(monophonicI->getControlPositionValue() == monoBeforeLoad);
      CHECK(monophonicII->getControlPositionValue() == Approx(0.75));
      CHECK(polyI->getControlPositionValue() == polyBeforeLoad);
      CHECK(polyII->getControlPositionValue() == Approx(0.75));
    }

    WHEN("II -> I")
    {
      auto polyBeforeLoad = polyII->getControlPositionValue();
      auto monoBeforeLoad = monophonicII->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::II, VoiceGroup::I);
      CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::II);

      CHECK(monophonicI->getControlPositionValue() == Approx(0.31));
      CHECK(monophonicII->getControlPositionValue() == monoBeforeLoad);
      CHECK(polyI->getControlPositionValue() == Approx(0.31));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);
    }

    WHEN("II -> II")
    {
      auto monoBeforeLoad = monophonicI->getControlPositionValue();
      auto polyBeforeLoad = polyI->getControlPositionValue();
      ebUseCases.loadToPart(single, VoiceGroup::II, VoiceGroup::II);
      CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == single->getUuid());
      CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);

      CHECK(monophonicI->getControlPositionValue() == monoBeforeLoad);
      CHECK(monophonicII->getControlPositionValue() == Approx(0.31));
      CHECK(polyI->getControlPositionValue() == polyBeforeLoad);
      CHECK(polyII->getControlPositionValue() == Approx(0.31));
    }
  }
}