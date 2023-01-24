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

  auto poly_I_preset_CP = 0.75;
  auto poly_II_preset_CP = 0.31;
  auto from_fx_preset_CP = 0.69;
  auto to_fx_preset_CP = 0.21;

  auto to_FX_I = eb->findParameterByID({C15::PID::Out_Mix_To_FX, VoiceGroup::I});
  auto to_FX_II = eb->findParameterByID({C15::PID::Out_Mix_To_FX, VoiceGroup::II});

  auto from_fx_I = eb->findParameterByID({C15::PID::FB_Mix_FX_Src, VoiceGroup::I});
  auto from_fx_II = eb->findParameterByID({C15::PID::FB_Mix_FX_Src, VoiceGroup::II});

  {
    ParameterUseCases puI(polyI);
    ParameterUseCases puII(polyII);
    ParameterUseCases muI(monophonicI);
    ParameterUseCases muII(monophonicII);
    ParameterUseCases toFXIUc(to_FX_I);
    ParameterUseCases toFXIIUc(to_FX_II);

    ParameterUseCases fromFXIUc(from_fx_I);
    ParameterUseCases fromFXIIUc(from_fx_II);

    ebUseCases.load(single);

    puI.setControlPosition(poly_I_preset_CP);
    puII.setControlPosition(poly_II_preset_CP);

    muI.setControlPosition(0.75);
    muII.setControlPosition(0.31);

    toFXIUc.setControlPosition(to_fx_preset_CP);
    toFXIIUc.setControlPosition(to_fx_preset_CP);

    fromFXIUc.setControlPosition(from_fx_preset_CP);
    fromFXIIUc.setControlPosition(from_fx_preset_CP);

    PresetUseCases puc(*single, eb->getSettings());
    puc.overwriteWithEditBuffer(*eb);

    CHECK(single->findParameterByID({ C15::PID::Env_A_BP, VoiceGroup::I }, true)->getValue() == Approx(poly_I_preset_CP));
    CHECK(single->findParameterByID({ C15::PID::Env_A_BP, VoiceGroup::II }, true)->getValue() == Approx(0.31));
    CHECK(single->findParameterByID({ C15::PID::Reverb_Chorus, VoiceGroup::I }, true)->getValue() == Approx(0.75));
    CHECK(single->findParameterByID({ C15::PID::Reverb_Chorus, VoiceGroup::II }, true)->getValue() == Approx(0.31));
    CHECK(single->findParameterByID({C15::PID::Out_Mix_To_FX, VoiceGroup::I}, true)->getValue() == Approx(to_fx_preset_CP));
    CHECK(single->findParameterByID({C15::PID::Out_Mix_To_FX, VoiceGroup::II}, true)->getValue() == Approx(to_fx_preset_CP));
    CHECK(single->findParameterByID({C15::PID::FB_Mix_FX_Src, VoiceGroup::I}, true)->getValue() == Approx(from_fx_preset_CP));
    CHECK(single->findParameterByID({C15::PID::FB_Mix_FX_Src, VoiceGroup::II}, true)->getValue() == Approx(from_fx_preset_CP));
  }

  WHEN("single preset is loaded into layer")
  {
    auto layer = presets.getLayerPreset();

    ebUseCases.load(layer);
    CHECK(eb->getPartOrigin(VoiceGroup::I).presetUUID == layer->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::I).sourceGroup == VoiceGroup::I);
    CHECK(eb->getPartOrigin(VoiceGroup::II).presetUUID == layer->getUuid());
    CHECK(eb->getPartOrigin(VoiceGroup::II).sourceGroup == VoiceGroup::II);

    CHECK(polyI->getControlPositionValue() != Approx(poly_I_preset_CP));
    CHECK(polyII->getControlPositionValue() != Approx(poly_II_preset_CP));
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

      CHECK(polyI->getControlPositionValue() == Approx(poly_I_preset_CP));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);

      CHECK(from_fx_II->isChangedFromLoaded() == false);
      CHECK(to_FX_II->isChangedFromLoaded() == false);
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
      CHECK(polyII->getControlPositionValue() == Approx(poly_I_preset_CP));

      CHECK(from_fx_I->isChangedFromLoaded() == false);
      CHECK(to_FX_I->isChangedFromLoaded() == false);
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

      CHECK(polyI->getControlPositionValue() == Approx(poly_I_preset_CP));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);

      CHECK(from_fx_II->isChangedFromLoaded() == false);
      CHECK(to_FX_II->isChangedFromLoaded() == false);
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
      CHECK(polyII->getControlPositionValue() == Approx(poly_I_preset_CP));

      CHECK(from_fx_I->isChangedFromLoaded() == false);
      CHECK(to_FX_I->isChangedFromLoaded() == false);
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

    CHECK(polyI->getControlPositionValue() != Approx(poly_I_preset_CP));
    CHECK(polyII->getControlPositionValue() != Approx(0.31));
    CHECK(monophonicI->getControlPositionValue() != Approx(poly_I_preset_CP));
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

      CHECK(polyI->getControlPositionValue() == Approx(poly_I_preset_CP));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);

      CHECK(from_fx_II->isChangedFromLoaded() == false);
      CHECK(to_FX_II->isChangedFromLoaded() == false);
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
      CHECK(polyII->getControlPositionValue() == Approx(poly_I_preset_CP));

      CHECK(from_fx_I->isChangedFromLoaded() == false);
      CHECK(to_FX_I->isChangedFromLoaded() == false);
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

      CHECK(polyI->getControlPositionValue() == Approx(poly_I_preset_CP));
      CHECK(polyII->getControlPositionValue() == polyBeforeLoad);

      CHECK(from_fx_II->isChangedFromLoaded() == false);
      CHECK(to_FX_II->isChangedFromLoaded() == false);
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
      CHECK(polyII->getControlPositionValue() == Approx(poly_I_preset_CP));

      CHECK(from_fx_I->isChangedFromLoaded() == false);
      CHECK(to_FX_I->isChangedFromLoaded() == false);
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Load to Part Load Single-Part II into Dual-Part I")
{
  MockPresetStorage presets;
  auto& eb = *TestHelper::getEditBuffer();
  auto& pm = *TestHelper::getPresetManager();
  auto& settings = *TestHelper::getSettings();

  EditBufferUseCases ebUseCases(eb);
  PresetManagerUseCases pmUseCases(pm, settings);

  auto polyI = eb.findParameterByID({C15::PID::FB_Mix_Asym, VoiceGroup::I});
  auto polyII = eb.findParameterByID({C15::PID::FB_Mix_Asym, VoiceGroup::II});

  auto monoI = eb.findParameterByID({C15::PID::Reverb_Mix, VoiceGroup::I});
  auto monoII = eb.findParameterByID({C15::PID::Reverb_Mix, VoiceGroup::II});

  WHEN("Prepared Single Preset!")
  {
    ebUseCases.load(presets.getSinglePreset());

    ParameterUseCases polyIUC(polyI);
    polyIUC.setControlPosition(0.2);

    ParameterUseCases polyIIUC(polyII);
    polyIIUC.setControlPosition(0.7);

    auto preparedSinglePresetBank = pmUseCases.createBankAndStoreEditBuffer();
    auto preparedSinglePreset = preparedSinglePresetBank->getPresetAt(0);
    REQUIRE(preparedSinglePresetBank);

    THEN("load split")
    {
      ebUseCases.load(presets.getSplitPreset());
      REQUIRE(polyI->getControlPositionValue() != Approx(0.2));
      REQUIRE(polyII->getControlPositionValue() != Approx(0.7));

      ebUseCases.loadToPart(preparedSinglePreset, VoiceGroup::II, VoiceGroup::I);
      REQUIRE(polyI->getControlPositionValue() == Approx(0.2)); // has value from Poly I
    }

    THEN("load layer")
    {
      ebUseCases.load(presets.getLayerPreset());
      REQUIRE(polyI->getControlPositionValue() != Approx(0.2));
      REQUIRE(polyII->getControlPositionValue() != Approx(0.7));

      ebUseCases.loadToPart(preparedSinglePreset, VoiceGroup::II, VoiceGroup::I);
      REQUIRE(polyI->getControlPositionValue() == Approx(0.2)); // has value from Poly I
    }
  }
}