#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "issue 3497")
{
  MockPresetStorage presets;
  auto& eb = *app->getPresetManager()->getEditBuffer();

  auto loadIntoSoundType = GENERATE(SoundType::Split, SoundType::Layer);

  EditBufferUseCases ebUseCases(eb);

  auto out_mix_to_fx_vg_I = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto out_mix_to_fx_vg_II = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });

  auto fb_mix_fx_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I });
  auto fb_mix_fx_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::II });

  auto fb_mix_fx_src_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto fb_mix_fx_src_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });

  auto fb_mix_a_b_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_Osc, VoiceGroup::I });
  auto fb_mix_a_b_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_Osc, VoiceGroup::II });

  auto fb_mix_comb_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_Comb, VoiceGroup::I });
  auto fb_mix_comb_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_Comb, VoiceGroup::II });

  auto fb_mix_comb_src_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_Comb_Src, VoiceGroup::I });
  auto fb_mix_comb_src_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_Comb_Src, VoiceGroup::II });

  auto fb_mix_svf_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_SVF, VoiceGroup::I });
  auto fb_mix_svf_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_SVF, VoiceGroup::II });

  auto fb_mix_svf_src_vg_I = eb.findParameterByID({ C15::PID::FB_Mix_SVF_Src, VoiceGroup::I });
  auto fb_mix_svf_src_vg_II = eb.findParameterByID({ C15::PID::FB_Mix_SVF_Src, VoiceGroup::II });

  WHEN("some sound of type is loaded " << toString(loadIntoSoundType))
  {
    ebUseCases.initSoundAs(loadIntoSoundType, Defaults::FactoryDefault);

    WHEN("loaded part of split preset into " << toString(loadIntoSoundType))
    {
      SECTION("loaded into I")
      {
        ParameterUseCases fbMixFx(fb_mix_fx_vg_I);
        ParameterUseCases fbMixFxSrc(fb_mix_fx_src_vg_I);
        ParameterUseCases fbComb(fb_mix_comb_vg_I);
        ParameterUseCases fbCombSrc(fb_mix_comb_src_vg_I);
        ParameterUseCases fbSvf(fb_mix_svf_vg_I);
        ParameterUseCases fbSvfSrc(fb_mix_svf_src_vg_I);

        fbMixFx.setControlPosition(0.18);
        fbMixFxSrc.setControlPosition(0.28);
        fbComb.setControlPosition(0.36);
        fbCombSrc.setControlPosition(0.87);
        fbSvf.setControlPosition(0.187);
        fbSvfSrc.setControlPosition(0.97);

        auto old_FB_MIX_FX_val = fb_mix_fx_vg_I->getControlPositionValue();
        auto old_FB_Mix_FX_src_val = fb_mix_fx_src_vg_I->getControlPositionValue();

        auto old_FB_MIX_Comb_val = fb_mix_comb_vg_I->getControlPositionValue();
        auto old_FB_MIX_Comb_src_val = fb_mix_comb_src_vg_I->getControlPositionValue();

        auto old_FB_MIX_SVF_val = fb_mix_svf_vg_I->getControlPositionValue();
        auto old_FB_MIX_SVF_src_val = fb_mix_svf_src_vg_I->getControlPositionValue();

        ebUseCases.loadToPart(presets.getSplitPreset(), VoiceGroup::I, VoiceGroup::I);

        CHECK(out_mix_to_fx_vg_I->getControlPositionValue() == 0);
        CHECK(fb_mix_fx_vg_I->getControlPositionValue()
              == fb_mix_fx_vg_I->getValue().getQuantizedValue(old_FB_MIX_FX_val * (1 - old_FB_Mix_FX_src_val), true));
        CHECK(fb_mix_fx_src_vg_I->getControlPositionValue() == 0);

        if(loadIntoSoundType == SoundType::Layer)
        {
          THEN("special into layer rules apply")
          {
            CHECK(fb_mix_a_b_vg_I->getControlPositionValue() == 0);
            CHECK(fb_mix_comb_vg_I->getControlPositionValue()
                  == fb_mix_comb_vg_I->getValue().getQuantizedValue(old_FB_MIX_Comb_val * (1 - old_FB_MIX_Comb_src_val),
                                                                    true));
            CHECK(fb_mix_comb_src_vg_I->getControlPositionValue() == 0);

            CHECK(fb_mix_svf_vg_I->getControlPositionValue()
                  == fb_mix_svf_vg_I->getValue().getQuantizedValue(old_FB_MIX_SVF_val * (1 - old_FB_MIX_SVF_src_val),
                                                                   true));
            CHECK(fb_mix_svf_src_vg_I->getControlPositionValue() == 0);
          }
        }
      }

      SECTION("loaded into II")
      {
        ParameterUseCases fbMixFx(fb_mix_fx_vg_II);
        ParameterUseCases fbMixFxSrc(fb_mix_fx_src_vg_II);
        ParameterUseCases fbComb(fb_mix_comb_vg_II);
        ParameterUseCases fbCombSrc(fb_mix_comb_src_vg_II);
        ParameterUseCases fbSvf(fb_mix_svf_vg_II);
        ParameterUseCases fbSvfSrc(fb_mix_svf_src_vg_II);

        fbMixFx.setControlPosition(0.18);
        fbMixFxSrc.setControlPosition(0.28);
        fbComb.setControlPosition(0.36);
        fbCombSrc.setControlPosition(0.87);
        fbSvf.setControlPosition(0.187);
        fbSvfSrc.setControlPosition(0.97);

        auto old_FB_MIX_FX_val = fb_mix_fx_vg_II->getControlPositionValue();
        auto old_FB_Mix_FX_src_val = fb_mix_fx_src_vg_II->getControlPositionValue();

        auto old_FB_MIX_Comb_val = fb_mix_comb_vg_II->getControlPositionValue();
        auto old_FB_MIX_Comb_src_val = fb_mix_comb_src_vg_II->getControlPositionValue();

        auto old_FB_MIX_SVF_val = fb_mix_svf_vg_II->getControlPositionValue();
        auto old_FB_MIX_SVF_src_val = fb_mix_svf_src_vg_II->getControlPositionValue();

        ebUseCases.loadToPart(presets.getSplitPreset(), VoiceGroup::I, VoiceGroup::II);

        CHECK(out_mix_to_fx_vg_II->getControlPositionValue() == 0);
        CHECK(fb_mix_fx_vg_II->getControlPositionValue()
              == fb_mix_fx_vg_II->getValue().getQuantizedValue(old_FB_MIX_FX_val * (1 - old_FB_Mix_FX_src_val), true));
        CHECK(fb_mix_fx_src_vg_II->getControlPositionValue() == 0);

        if(loadIntoSoundType == SoundType::Layer)
        {

          THEN("special into layer rules apply")
          {
            CHECK(fb_mix_a_b_vg_II->getControlPositionValue() == 0);
            CHECK(fb_mix_comb_vg_II->getControlPositionValue()
                  == fb_mix_comb_vg_II->getValue().getQuantizedValue(
                      old_FB_MIX_Comb_val * (1 - old_FB_MIX_Comb_src_val), true));
            CHECK(fb_mix_comb_src_vg_II->getControlPositionValue() == 0);

            CHECK(fb_mix_svf_vg_II->getControlPositionValue()
                  == fb_mix_svf_vg_II->getValue().getQuantizedValue(old_FB_MIX_SVF_val * (1 - old_FB_MIX_SVF_src_val),
                                                                    true));
            CHECK(fb_mix_svf_src_vg_II->getControlPositionValue() == 0);
          }
        }
      }
    }

    WHEN("loaded part of layer preset into " << toString(loadIntoSoundType))
    {
      SECTION("loaded into I")
      {
        ParameterUseCases fbMixFx(fb_mix_fx_vg_I);
        ParameterUseCases fbMixFxSrc(fb_mix_fx_src_vg_I);
        ParameterUseCases fbComb(fb_mix_comb_vg_I);
        ParameterUseCases fbCombSrc(fb_mix_comb_src_vg_I);
        ParameterUseCases fbSvf(fb_mix_svf_vg_I);
        ParameterUseCases fbSvfSrc(fb_mix_svf_src_vg_I);

        fbMixFx.setControlPosition(0.18);
        fbMixFxSrc.setControlPosition(0.28);
        fbComb.setControlPosition(0.36);
        fbCombSrc.setControlPosition(0.87);
        fbSvf.setControlPosition(0.187);
        fbSvfSrc.setControlPosition(0.97);

        auto old_FB_MIX_FX_val = fb_mix_fx_vg_I->getControlPositionValue();
        auto old_FB_Mix_FX_src_val = fb_mix_fx_src_vg_I->getControlPositionValue();

        auto old_FB_MIX_Comb_val = fb_mix_comb_vg_I->getControlPositionValue();
        auto old_FB_MIX_Comb_src_val = fb_mix_comb_src_vg_I->getControlPositionValue();

        auto old_FB_MIX_SVF_val = fb_mix_svf_vg_I->getControlPositionValue();
        auto old_FB_MIX_SVF_src_val = fb_mix_svf_src_vg_I->getControlPositionValue();

        ebUseCases.loadToPart(presets.getLayerPreset(), VoiceGroup::I, VoiceGroup::I);

        CHECK(out_mix_to_fx_vg_I->getControlPositionValue() == 0);
        CHECK(fb_mix_fx_vg_I->getControlPositionValue()
              == fb_mix_fx_vg_I->getValue().getQuantizedValue(old_FB_MIX_FX_val * (1 - old_FB_Mix_FX_src_val), true));
        CHECK(fb_mix_fx_src_vg_I->getControlPositionValue() == 0);

        if(loadIntoSoundType == SoundType::Layer)
        {

          THEN("special into layer rules apply")
          {
            CHECK(fb_mix_a_b_vg_I->getControlPositionValue() == 0);
            CHECK(fb_mix_comb_vg_I->getControlPositionValue()
                  == fb_mix_comb_vg_I->getValue().getQuantizedValue(old_FB_MIX_Comb_val * (1 - old_FB_MIX_Comb_src_val),
                                                                    true));
            CHECK(fb_mix_comb_src_vg_I->getControlPositionValue() == 0);

            CHECK(fb_mix_svf_vg_I->getControlPositionValue()
                  == fb_mix_svf_vg_I->getValue().getQuantizedValue(old_FB_MIX_SVF_val * (1 - old_FB_MIX_SVF_src_val),
                                                                   true));
            CHECK(fb_mix_svf_src_vg_I->getControlPositionValue() == 0);
          }
        }
      }

      SECTION("loaded into II")
      {
        ParameterUseCases fbMixFx(fb_mix_fx_vg_II);
        ParameterUseCases fbMixFxSrc(fb_mix_fx_src_vg_II);
        ParameterUseCases fbComb(fb_mix_comb_vg_II);
        ParameterUseCases fbCombSrc(fb_mix_comb_src_vg_II);
        ParameterUseCases fbSvf(fb_mix_svf_vg_II);
        ParameterUseCases fbSvfSrc(fb_mix_svf_src_vg_II);

        fbMixFx.setControlPosition(0.18);
        fbMixFxSrc.setControlPosition(0.28);
        fbComb.setControlPosition(0.36);
        fbCombSrc.setControlPosition(0.87);
        fbSvf.setControlPosition(0.187);
        fbSvfSrc.setControlPosition(0.97);

        auto old_FB_MIX_FX_val = fb_mix_fx_vg_II->getControlPositionValue();
        auto old_FB_Mix_FX_src_val = fb_mix_fx_src_vg_II->getControlPositionValue();

        auto old_FB_MIX_Comb_val = fb_mix_comb_vg_II->getControlPositionValue();
        auto old_FB_MIX_Comb_src_val = fb_mix_comb_src_vg_II->getControlPositionValue();

        auto old_FB_MIX_SVF_val = fb_mix_svf_vg_II->getControlPositionValue();
        auto old_FB_MIX_SVF_src_val = fb_mix_svf_src_vg_II->getControlPositionValue();

        ebUseCases.loadToPart(presets.getLayerPreset(), VoiceGroup::I, VoiceGroup::II);

        CHECK(out_mix_to_fx_vg_II->getControlPositionValue() == 0);
        CHECK(fb_mix_fx_vg_II->getControlPositionValue()
              == fb_mix_fx_vg_II->getValue().getQuantizedValue(old_FB_MIX_FX_val * (1 - old_FB_Mix_FX_src_val), true));
        CHECK(fb_mix_fx_src_vg_II->getControlPositionValue() == 0);

        //layer specific rules
        if(loadIntoSoundType == SoundType::Layer)
        {

          THEN("special into layer rules apply")
          {
            CHECK(fb_mix_a_b_vg_II->getControlPositionValue() == 0);
            CHECK(fb_mix_comb_vg_II->getControlPositionValue()
                  == fb_mix_comb_vg_II->getValue().getQuantizedValue(
                      old_FB_MIX_Comb_val * (1 - old_FB_MIX_Comb_src_val), true));
            CHECK(fb_mix_comb_src_vg_II->getControlPositionValue() == 0);

            CHECK(fb_mix_svf_vg_II->getControlPositionValue()
                  == fb_mix_svf_vg_II->getValue().getQuantizedValue(old_FB_MIX_SVF_val * (1 - old_FB_MIX_SVF_src_val),
                                                                    true));
            CHECK(fb_mix_svf_src_vg_II->getControlPositionValue() == 0);
          }
        }
      }
    }

    WHEN("loaded part of single preset into " << toString(loadIntoSoundType))
    {
      SECTION("loaded into I")
      {
        ParameterUseCases fbMixFx(fb_mix_fx_vg_I);
        ParameterUseCases fbMixFxSrc(fb_mix_fx_src_vg_I);
        ParameterUseCases fbComb(fb_mix_comb_vg_I);
        ParameterUseCases fbCombSrc(fb_mix_comb_src_vg_I);
        ParameterUseCases fbSvf(fb_mix_svf_vg_I);
        ParameterUseCases fbSvfSrc(fb_mix_svf_src_vg_I);

        fbMixFx.setControlPosition(0.18);
        fbMixFxSrc.setControlPosition(0.28);
        fbComb.setControlPosition(0.36);
        fbCombSrc.setControlPosition(0.87);
        fbSvf.setControlPosition(0.187);
        fbSvfSrc.setControlPosition(0.97);

        auto old_FB_MIX_FX_val = fb_mix_fx_vg_I->getControlPositionValue();
        auto old_FB_Mix_FX_src_val = fb_mix_fx_src_vg_I->getControlPositionValue();

        auto old_FB_MIX_Comb_val = fb_mix_comb_vg_I->getControlPositionValue();
        auto old_FB_MIX_Comb_src_val = fb_mix_comb_src_vg_I->getControlPositionValue();

        auto old_FB_MIX_SVF_val = fb_mix_svf_vg_I->getControlPositionValue();
        auto old_FB_MIX_SVF_src_val = fb_mix_svf_src_vg_I->getControlPositionValue();

        ebUseCases.loadToPart(presets.getSinglePreset(), VoiceGroup::I, VoiceGroup::I);

        CHECK(out_mix_to_fx_vg_I->getControlPositionValue() == 0);
        CHECK(fb_mix_fx_vg_I->getControlPositionValue() == fb_mix_fx_vg_I->getValue().getQuantizedValue(old_FB_MIX_FX_val * (1 - old_FB_Mix_FX_src_val), true));
        CHECK(fb_mix_fx_src_vg_I->getControlPositionValue() == 0);

        if(loadIntoSoundType == SoundType::Layer)
        {
          CHECK(fb_mix_a_b_vg_I->getControlPositionValue() == 0);
          CHECK(fb_mix_comb_vg_I->getControlPositionValue() == fb_mix_comb_vg_I->getValue().getQuantizedValue(old_FB_MIX_Comb_val * (1 - old_FB_MIX_Comb_src_val), true));
          CHECK(fb_mix_comb_src_vg_I->getControlPositionValue() == 0);

          CHECK(fb_mix_svf_vg_I->getControlPositionValue() == fb_mix_svf_vg_I->getValue().getQuantizedValue(old_FB_MIX_SVF_val * (1 - old_FB_MIX_SVF_src_val), true));
          CHECK(fb_mix_svf_src_vg_I->getControlPositionValue() == 0);
        }
      }

      SECTION("loaded into II")
      {
        ParameterUseCases fbMixFx(fb_mix_fx_vg_II);
        ParameterUseCases fbMixFxSrc(fb_mix_fx_src_vg_II);
        ParameterUseCases fbComb(fb_mix_comb_vg_II);
        ParameterUseCases fbCombSrc(fb_mix_comb_src_vg_II);
        ParameterUseCases fbSvf(fb_mix_svf_vg_II);
        ParameterUseCases fbSvfSrc(fb_mix_svf_src_vg_II);

        fbMixFx.setControlPosition(0.18);
        fbMixFxSrc.setControlPosition(0.28);
        fbComb.setControlPosition(0.36);
        fbCombSrc.setControlPosition(0.87);
        fbSvf.setControlPosition(0.187);
        fbSvfSrc.setControlPosition(0.97);

        auto old_FB_MIX_FX_val = fb_mix_fx_vg_II->getControlPositionValue();
        auto old_FB_Mix_FX_src_val = fb_mix_fx_src_vg_II->getControlPositionValue();

        auto old_FB_MIX_Comb_val = fb_mix_comb_vg_II->getControlPositionValue();
        auto old_FB_MIX_Comb_src_val = fb_mix_comb_src_vg_II->getControlPositionValue();

        auto old_FB_MIX_SVF_val = fb_mix_svf_vg_II->getControlPositionValue();
        auto old_FB_MIX_SVF_src_val = fb_mix_svf_src_vg_II->getControlPositionValue();

        ebUseCases.loadToPart(presets.getSinglePreset(), VoiceGroup::I, VoiceGroup::II);

        CHECK(out_mix_to_fx_vg_II->getControlPositionValue() == 0);
        CHECK(fb_mix_fx_vg_II->getControlPositionValue() == fb_mix_fx_vg_II->getValue().getQuantizedValue(old_FB_MIX_FX_val * (1 - old_FB_Mix_FX_src_val), true));
        CHECK(fb_mix_fx_src_vg_II->getControlPositionValue() == 0);

        if(loadIntoSoundType == SoundType::Layer)
        {
          CHECK(fb_mix_a_b_vg_II->getControlPositionValue() == 0);
          CHECK(fb_mix_comb_vg_II->getControlPositionValue() == fb_mix_comb_vg_II->getValue().getQuantizedValue(old_FB_MIX_Comb_val * (1 - old_FB_MIX_Comb_src_val), true));
          CHECK(fb_mix_comb_src_vg_II->getControlPositionValue() == 0);

          CHECK(fb_mix_svf_vg_II->getControlPositionValue() == fb_mix_svf_vg_II->getValue().getQuantizedValue(old_FB_MIX_SVF_val * (1 - old_FB_MIX_SVF_src_val), true));
          CHECK(fb_mix_svf_src_vg_II->getControlPositionValue() == 0);
        }
      }
    }
  }
}