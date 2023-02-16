#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "load-to-part cross-fx-reset new rule-set")
{
  MockPresetStorage presets;
  auto& eb = *app->getPresetManager()->getEditBuffer();

  EditBufferUseCases ebUseCases(eb);

  auto out_mix_lvl_vg_I = eb.findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I });
  auto out_mix_lvl_vg_II = eb.findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::II });

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

  auto fb_mix_osc_I = eb.findParameterByID({ C15::PID::FB_Mix_Osc, VoiceGroup::I });
  auto fb_mix_osc_II = eb.findParameterByID({ C15::PID::FB_Mix_Osc, VoiceGroup::II });

  auto fb_mix_osc_src_I = eb.findParameterByID({ C15::PID::FB_Mix_Osc_Src, VoiceGroup::I });
  auto fb_mix_osc_src_II = eb.findParameterByID({ C15::PID::FB_Mix_Osc_Src, VoiceGroup::II });

  auto setPresetValues = [&](Preset* p, VoiceGroup src) {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    p->findParameterByID({ C15::PID::FB_Mix_Osc, src }, false)->setValue(transaction, 1);
    p->findParameterByID({ C15::PID::FB_Mix_Osc_Src, src }, false)->setValue(transaction, 0.1);
    p->findParameterByID({ C15::PID::FB_Mix_Comb, src }, false)->setValue(transaction, 0.36);
    p->findParameterByID({ C15::PID::FB_Mix_Comb_Src, src }, false)->setValue(transaction, 0.87);
    p->findParameterByID({ C15::PID::FB_Mix_SVF, src }, false)->setValue(transaction, 0.187);
    p->findParameterByID({ C15::PID::FB_Mix_SVF_Src, src }, false)->setValue(transaction, 0.97);
    p->findParameterByID({ C15::PID::FB_Mix_FX, src }, false)->setValue(transaction, 0.18);
    p->findParameterByID({ C15::PID::FB_Mix_FX_Src, src }, false)->setValue(transaction, 0.28);
    p->findParameterByID({ C15::PID::Out_Mix_Lvl, src }, false)->setValue(transaction, 0.38);
    p->findParameterByID({ C15::PID::Out_Mix_To_FX, src }, false)->setValue(transaction, 0.1);
  };

  auto quantizeApprox = [](Parameter* p, double cp) { return p->getValue().getQuantizedValue(cp, true); };

  WHEN("layer sound is loaded")
  {
    ebUseCases.initSoundAs(SoundType::Layer, Defaults::FactoryDefault);

    auto loadTarget = GENERATE(VoiceGroup::I, VoiceGroup::II);

    auto out_mix_lvl = eb.findParameterByID({ C15::PID::Out_Mix_Lvl, loadTarget });
    auto out_mix_to_fx = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, loadTarget });

    auto fb_mix_fx = eb.findParameterByID({ C15::PID::FB_Mix_FX, loadTarget });
    auto fb_mix_fx_src = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, loadTarget });
    auto fb_mix_osc = eb.findParameterByID({ C15::PID::FB_Mix_Osc, loadTarget });
    auto fb_mix_osc_src = eb.findParameterByID({ C15::PID::FB_Mix_Osc_Src, loadTarget });
    auto fb_mix_comb = eb.findParameterByID({ C15::PID::FB_Mix_Comb, loadTarget });
    auto fb_mix_comb_src = eb.findParameterByID({ C15::PID::FB_Mix_Comb_Src, loadTarget });
    auto fb_mix_svf = eb.findParameterByID({ C15::PID::FB_Mix_SVF, loadTarget });
    auto fb_mix_svf_src = eb.findParameterByID({ C15::PID::FB_Mix_SVF_Src, loadTarget });

    THEN("load single part I into " << toString(loadTarget) << " of layer")
    {
      setPresetValues(presets.getSinglePreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getSinglePreset(), VoiceGroup::I, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152733));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load single part II into " << toString(loadTarget) << " of layer")
    {
      setPresetValues(presets.getSinglePreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getSinglePreset(), VoiceGroup::II, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.095247));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.120167));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load split part I into " << toString(loadTarget) << " of layer")
    {
      setPresetValues(presets.getSplitPreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getSplitPreset(), VoiceGroup::I, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.0468));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.00560999));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load split part II into " << toString(loadTarget) << " of layer")
    {
      setPresetValues(presets.getSplitPreset(), VoiceGroup::II);
      ebUseCases.loadToPart(presets.getSplitPreset(), VoiceGroup::II, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.0468));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.00560999));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load layer part I into " << toString(loadTarget) << " of layer")
    {
      setPresetValues(presets.getLayerPreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getLayerPreset(), VoiceGroup::I, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.0468));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.00560999));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load layer part II into " << toString(loadTarget) << " of layer")
    {
      setPresetValues(presets.getLayerPreset(), VoiceGroup::II);
      ebUseCases.loadToPart(presets.getLayerPreset(), VoiceGroup::II, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.0468));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.00560999));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }
  }

  WHEN("split sound is loaded")
  {
    ebUseCases.initSoundAs(SoundType::Split, Defaults::FactoryDefault);

    auto loadTarget = GENERATE(VoiceGroup::I, VoiceGroup::II);

    auto out_mix_lvl = eb.findParameterByID({ C15::PID::Out_Mix_Lvl, loadTarget });
    auto out_mix_to_fx = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, loadTarget });

    auto fb_mix_fx = eb.findParameterByID({ C15::PID::FB_Mix_FX, loadTarget });
    auto fb_mix_fx_src = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, loadTarget });
    auto fb_mix_osc = eb.findParameterByID({ C15::PID::FB_Mix_Osc, loadTarget });
    auto fb_mix_osc_src = eb.findParameterByID({ C15::PID::FB_Mix_Osc_Src, loadTarget });
    auto fb_mix_comb = eb.findParameterByID({ C15::PID::FB_Mix_Comb, loadTarget });
    auto fb_mix_comb_src = eb.findParameterByID({ C15::PID::FB_Mix_Comb_Src, loadTarget });
    auto fb_mix_svf = eb.findParameterByID({ C15::PID::FB_Mix_SVF, loadTarget });
    auto fb_mix_svf_src = eb.findParameterByID({ C15::PID::FB_Mix_SVF_Src, loadTarget });

    THEN("load single part I into " << toString(loadTarget) << " of split")
    {
      setPresetValues(presets.getSinglePreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getSinglePreset(), VoiceGroup::I, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152733));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load single part II into " << toString(loadTarget) << " of split")
    {
      setPresetValues(presets.getSinglePreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getSinglePreset(), VoiceGroup::II, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.095247));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.120167));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load split part I into " << toString(loadTarget) << " of split")
    {
      setPresetValues(presets.getSplitPreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getSplitPreset(), VoiceGroup::I, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load split part II into " << toString(loadTarget) << " of split")
    {
      setPresetValues(presets.getSplitPreset(), VoiceGroup::II);
      ebUseCases.loadToPart(presets.getSplitPreset(), VoiceGroup::II, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load layer part I into " << toString(loadTarget) << " of split")
    {
      setPresetValues(presets.getLayerPreset(), VoiceGroup::I);
      ebUseCases.loadToPart(presets.getLayerPreset(), VoiceGroup::I, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }

    THEN("load layer part II into " << toString(loadTarget) << " of split")
    {
      setPresetValues(presets.getLayerPreset(), VoiceGroup::II);
      ebUseCases.loadToPart(presets.getLayerPreset(), VoiceGroup::II, loadTarget);

      CHECK(fb_mix_osc->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_osc_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_comb->getControlPositionValue() == quantizeApprox(fb_mix_comb, 0.36));
      CHECK(fb_mix_comb_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_svf->getControlPositionValue() == quantizeApprox(fb_mix_svf, 0.187));
      CHECK(fb_mix_svf_src->getControlPositionValue() == Approx(0));
      CHECK(fb_mix_fx->getControlPositionValue() == quantizeApprox(fb_mix_fx, 0.152735));
      CHECK(fb_mix_fx_src->getControlPositionValue() == Approx(0));

      CHECK(out_mix_lvl->getControlPositionValue() == quantizeApprox(out_mix_lvl, 0.36));
      CHECK(out_mix_to_fx->getControlPositionValue() == Approx(0));
    }
  }
}