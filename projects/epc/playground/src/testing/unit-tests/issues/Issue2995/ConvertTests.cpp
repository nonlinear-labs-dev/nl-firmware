#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include <testing/unit-tests/mock/EditBufferNamedLogicalParts.h>

namespace
{
  VoiceGroup invert(VoiceGroup vg)
  {
    return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
  }

  Parameter* getOtherPolyParam(Parameter* p, EditBuffer& eb)
  {
    if(p->getID().getVoiceGroup() == VoiceGroup::Global)
      return nullptr;
    return eb.findParameterByID({ p->getID().getNumber(), invert(p->getID().getVoiceGroup()) });
  }

  std::vector<Parameter*> getOtherPolyVector(std::vector<Parameter*> v, EditBuffer& eb)
  {
    std::vector<Parameter*> ret;
    for(auto& p: v)
    {
      ret.emplace_back(getOtherPolyParam(p, eb));
    }
    return ret;
  }

  std::vector<Parameter*> collectParams(const std::vector<ParameterId>& ids, EditBuffer& eb)
  {
    std::vector<Parameter*> ret;
    for(auto i: ids)
    {
      ret.emplace_back(eb.findParameterByID(i));
    }
    return ret;
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert sounds -> dual fx")
{
  using ELP = EditBufferLogicalParts;
  auto& eb = *TestHelper::getEditBuffer();
  MockPresetStorage presets;

  EditBufferUseCases ebUseCases(eb);

  auto fb_mix_osc = eb.findParameterByID({ C15::PID::FB_Mix_Osc, VoiceGroup::I });
  auto fb_mix_osc_src = eb.findParameterByID({ C15::PID::FB_Mix_Osc_Src, VoiceGroup::I });
  auto fb_mix_comb_from = eb.findParameterByID({ C15::PID::FB_Mix_Comb_Src, VoiceGroup::I });
  auto fb_mix_svf_from = eb.findParameterByID({ C15::PID::FB_Mix_SVF_Src, VoiceGroup::I });

  auto fb_mix_fx_from = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });

  auto fb_mix_other = eb.findParameterByID({ C15::PID::FB_Mix_Asym, VoiceGroup::I });

  auto out_mix_to_other = eb.findParameterByID({ C15::PID::Out_Mix_A_Lvl, VoiceGroup::I });

  auto flanger_other_I = eb.findParameterByID({ C15::PID::Flanger_Time, VoiceGroup::I });
  auto flanger_other_II = eb.findParameterByID({ C15::PID::Flanger_Time, VoiceGroup::II });
  auto reverb_other_I = eb.findParameterByID({ C15::PID::Reverb_Color, VoiceGroup::I });
  auto reverb_other_II = eb.findParameterByID({ C15::PID::Reverb_Color, VoiceGroup::II });
  auto cabinet_other_I = eb.findParameterByID({ C15::PID::Cabinet_Drive, VoiceGroup::I });
  auto cabinet_other_II = eb.findParameterByID({ C15::PID::Cabinet_Drive, VoiceGroup::II });
  auto echo_other_I = eb.findParameterByID({ C15::PID::Echo_Hi_Cut, VoiceGroup::I });
  auto echo_other_II = eb.findParameterByID({ C15::PID::Echo_Hi_Cut, VoiceGroup::II });
  auto gap_other_I = eb.findParameterByID({ C15::PID::Gap_Flt_Gap, VoiceGroup::I });
  auto gap_other_II = eb.findParameterByID({ C15::PID::Gap_Flt_Gap, VoiceGroup::II });

  auto voicesI = eb.findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::I });
  auto voicesII = eb.findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::II });

  auto mono_some = eb.findParameterByID({ C15::PID::Mono_Grp_Glide, VoiceGroup::I });

  auto master_tune = eb.findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global });

  auto split_I = eb.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto split_II = eb.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II });

  auto fade_rng_I = eb.findParameterByID({ C15::PID::Voice_Grp_Fade_Range, VoiceGroup::I });
  auto fade_rng_II = eb.findParameterByID({ C15::PID::Voice_Grp_Fade_Range, VoiceGroup::II });

  auto fade_from_I = eb.findParameterByID({ C15::PID::Voice_Grp_Fade_From, VoiceGroup::I });
  auto fade_from_II = eb.findParameterByID({ C15::PID::Voice_Grp_Fade_From, VoiceGroup::II });

  auto unison_voices_I = eb.findParameterByID({C15::PID::Unison_Voices, VoiceGroup::I});
  auto unison_detune_I = eb.findParameterByID({C15::PID::Unison_Detune, VoiceGroup::I});
  auto unison_pan_I = eb.findParameterByID({C15::PID::Unison_Pan, VoiceGroup::I});
  auto unison_phase_I = eb.findParameterByID({C15::PID::Unison_Phase, VoiceGroup::I});

  auto mono_enable_I = eb.findParameterByID({C15::PID::Mono_Grp_Enable, VoiceGroup::I});
  auto mono_glide_I = eb.findParameterByID({C15::PID::Mono_Grp_Glide, VoiceGroup::I});
  auto mono_legato_I = eb.findParameterByID({C15::PID::Mono_Grp_Legato, VoiceGroup::I});
  auto mono_prio_I = eb.findParameterByID({C15::PID::Mono_Grp_Prio, VoiceGroup::I});

  auto part_vol_I = eb.findParameterByID({C15::PID::Voice_Grp_Volume, VoiceGroup::I});
  auto part_tune_I = eb.findParameterByID({C15::PID::Voice_Grp_Tune, VoiceGroup::I});

  WHEN("Single preset is prepared and loaded")
  {
    ebUseCases.load(presets.getSinglePreset());
    ebUseCases.randomize(0.1);

    ParameterUseCases voicesUseCases(voicesI);
    voicesUseCases.setControlPosition(1);

    REQUIRE(voicesI->getDisplayString() == "24 voices");

    WHEN("convert to split")
    {

      std::vector<Parameter*> shouldBeDefaulted
          = { fb_mix_osc, fb_mix_osc_src, fb_mix_comb_from, fb_mix_svf_from, split_I,
              split_II,   fade_from_I,    fade_from_II,     fade_rng_I,      fade_rng_II };

      std::vector<Parameter*> shouldBeTransferredToIandII = { fb_mix_other, out_mix_to_other, mono_some };

      std::vector<Parameter*> shouldBeKeptAsIs
          = { flanger_other_I,  flanger_other_II, reverb_other_I, reverb_other_II, cabinet_other_I,
              cabinet_other_II, echo_other_I,     echo_other_II,  gap_other_I,     gap_other_II };

      auto totalHashOfAllParametersKeptAsIs = ELP::createHashOfVector(shouldBeKeptAsIs);
      auto oldMasterTune = master_tune->getDisplayString();
      ebUseCases.convertToDual(SoundType::Split, VoiceGroup::I);

      THEN("Check FX from")
      {
        //TODO
      }

      THEN("Part Volume")
      {
        //TODO
      }

      THEN("part.tune was transferred from master.tune")
      {
        CHECK(eb.findParameterByID({ C15::PID::Voice_Grp_Tune, VoiceGroup::I })->getDisplayString() == oldMasterTune);
        CHECK(eb.findParameterByID({ C15::PID::Voice_Grp_Tune, VoiceGroup::II })->getDisplayString() == oldMasterTune);
      }

      THEN("voices are half each")
      {
        CHECK(voicesI->getDisplayString() == "12 voices");
        CHECK(voicesII->getDisplayString() == "12 voices");
      }

      THEN("defaulted parameters are correct")
      {
        for(auto& p : shouldBeDefaulted)
        {
          INFO("after convert to split " << p->getLongName()
                                         << " is not default! but:" << p->getControlPositionValue());
          CHECK(p->isDefaultLoaded());
        }
      }

      THEN("I was copied into II")
      {
        for(auto& og : shouldBeTransferredToIandII)
        {
          if(auto other = getOtherPolyParam(og, eb))
          {
            INFO("after convert to split " << other->getLongName()
                                           << " was not copied from I! got:" << other->getControlPositionValue()
                                           << " should: " << og->getControlPositionValue());
            CHECK(other->getControlPositionValue() == og->getControlPositionValue());
          }
        }
      }

      THEN("Parameters not touched")  //fails when the fx groups are not yet declared monophonic
      {
        auto totalHashOfAllParametersKeptAsIsAfterConvert = ELP::createHashOfVector(shouldBeKeptAsIs);
        CHECK(totalHashOfAllParametersKeptAsIs == totalHashOfAllParametersKeptAsIsAfterConvert);
      }
    }

    WHEN("convert to layer")
    {
      std::vector<Parameter*> shouldBeDefaulted
          = { fb_mix_osc, fb_mix_osc_src, fb_mix_comb_from, fb_mix_svf_from, split_I,
              split_II,   fade_from_I,    fade_from_II,     fade_rng_I,      fade_rng_II };

      std::vector<Parameter*> shouldBeTransferredToIandII = { fb_mix_other, out_mix_to_other };

      std::vector<Parameter*> shouldBeKeptAsIs
          = { flanger_other_I,  flanger_other_II, reverb_other_I, reverb_other_II, cabinet_other_I,
              cabinet_other_II, echo_other_I,     echo_other_II,  gap_other_I,     gap_other_II };

      auto totalHashOfAllParametersKeptAsIs = ELP::createHashOfVector(shouldBeKeptAsIs);
      auto oldMasterTune = master_tune->getDisplayString();

      ebUseCases.convertToDual(SoundType::Layer, VoiceGroup::I);

      THEN("Check FX from")
      {
        //TODO
      }

      THEN("Part Volume")
      {
        //TODO
      }

      THEN("part.tune was transferred from master.tune")
      {
        CHECK(eb.findParameterByID({ C15::PID::Voice_Grp_Tune, VoiceGroup::I })->getDisplayString() == oldMasterTune);
        CHECK(eb.findParameterByID({ C15::PID::Voice_Grp_Tune, VoiceGroup::II })->getDisplayString() == oldMasterTune);
      }

      THEN("voices are halfed")
      {
        CHECK(voicesI->getDisplayString() == "12 voices");
      }

      THEN("defaulted parameters are corrent")
      {
        for(auto& p : shouldBeDefaulted)
        {
          INFO("after convert to split " << p->getLongName()
                                         << " is not default! but:" << p->getControlPositionValue());
          CHECK(p->isDefaultLoaded());
        }
      }

      THEN("I was copied into II")
      {
        for(auto& og : shouldBeTransferredToIandII)
        {
          if(auto other = getOtherPolyParam(og, eb))
          {
            INFO("after convert to split " << other->getLongName()
                                           << " was not copied from I! got:" << other->getControlPositionValue()
                                           << " should: " << og->getControlPositionValue());
            CHECK(other->getControlPositionValue() == og->getControlPositionValue());
          }
        }
      }

      THEN("Parameters not touched")  //fails when the fx groups are not yet declared monophonic
      {
        auto totalHashOfAllParametersKeptAsIsAfterConvert = ELP::createHashOfVector(shouldBeKeptAsIs);
        CHECK(totalHashOfAllParametersKeptAsIs == totalHashOfAllParametersKeptAsIsAfterConvert);
      }
    }
  }

  WHEN("Split preset is prepared and loaded")
  {
    auto split = presets.getSplitPreset();
    ebUseCases.load(split);
    ebUseCases.randomize(0.1);

    WHEN("convert I to single")
    {
      std::vector<Parameter*> shouldBeTransferredFromSourceToI = { fb_mix_other, out_mix_to_other, mono_some };
      std::vector<Parameter*> shouldBeDefaultedI = {fb_mix_osc, fb_mix_osc_src, fb_mix_svf_from, split_I, fade_rng_I, fade_from_I, part_tune_I, part_vol_I};
      std::vector<Parameter*> shouldBeDefaultedII = getOtherPolyVector(shouldBeDefaultedI, eb);

      std::vector<Parameter*> unisonAndMonoI = {unison_detune_I, unison_pan_I, unison_phase_I, mono_enable_I, mono_glide_I, mono_legato_I, mono_prio_I}; //without voices

      std::vector<Parameter*> shouldBeKeptAsIs
          = { flanger_other_I,  flanger_other_II, reverb_other_I, reverb_other_II, cabinet_other_I,
              cabinet_other_II, echo_other_I,     echo_other_II,  gap_other_I,     gap_other_II };

      auto totalHashOfAllParametersKeptAsIs = ELP::createHashOfVector(shouldBeKeptAsIs);
      auto hashOfPolyGroup = ELP::createHashOfVector(shouldBeTransferredFromSourceToI);
      auto oldFB_OtherValue = fb_mix_other->getControlPositionValue();
      auto valueHashOfSourceUnisonAndMono = ELP::createValueHash(unisonAndMonoI);

      ebUseCases.convertToSingle(VoiceGroup::I);

      THEN("Poly parts were copied from src group to I")
      {
        auto hashOfPolyGroupAfterConvert = ELP::createHashOfVector(shouldBeTransferredFromSourceToI);
        CHECK(hashOfPolyGroup == hashOfPolyGroupAfterConvert);
      }

      THEN("params are defaulted")
      {
        for(auto& vec: {shouldBeDefaultedI, shouldBeDefaultedII})
        {
          for(auto p: vec)
          {
            INFO("should be defaulted " << p->getLongName() << " " << toString(p->getID().getVoiceGroup()) << " but isn't.");
            CHECK(p->isDefaultLoaded());
          }
        }
      }

      THEN("FX from is transfered")
      {
        //TODO implement
      }

      THEN("FB_MIX other was copied from src to I")
      {
        CHECK(fb_mix_other->getControlPositionValue() == oldFB_OtherValue);
      }

      THEN("FX groups left alone")
      {
        auto totalHashOfAllParametersKeptAsIsAfterConvert = ELP::createHashOfVector(shouldBeKeptAsIs);
        CHECK(totalHashOfAllParametersKeptAsIs == totalHashOfAllParametersKeptAsIsAfterConvert);
      }

      THEN("unison copied from src to -> I")
      {
        auto valueHashOfUnisonAndMonoAfterConvert = ELP::createValueHash(unisonAndMonoI);
        CHECK(valueHashOfSourceUnisonAndMono == valueHashOfUnisonAndMonoAfterConvert);
      }
    }

    WHEN("convert II to single")
    {
      std::vector<Parameter*> polyParametersI = {fb_mix_other, out_mix_to_other, mono_some};
      std::vector<Parameter*> shouldBeTransferredFromIIToI = getOtherPolyVector(polyParametersI, eb);
      std::vector<Parameter*> shouldBeDefaultedI = {fb_mix_osc, fb_mix_osc_src, fb_mix_svf_from, split_I, fade_rng_I, fade_from_I};
      std::vector<Parameter*> shouldBeDefaultedII = getOtherPolyVector(shouldBeDefaultedI, eb);
      std::vector<Parameter*> unisonAndMonoI = {unison_detune_I, unison_pan_I, unison_phase_I, mono_enable_I, mono_glide_I, mono_legato_I, mono_prio_I}; //without voices!!!
      auto unisonAndMonoII = getOtherPolyVector(unisonAndMonoI, eb);

      std::vector<Parameter*> shouldBeKeptAsIs
          = { flanger_other_I,  flanger_other_II, reverb_other_I, reverb_other_II, cabinet_other_I,
              cabinet_other_II, echo_other_I,     echo_other_II,  gap_other_I,     gap_other_II };

      auto totalHashOfAllParametersKeptAsIs = ELP::createHashOfVector(shouldBeKeptAsIs);
      auto hashOfPolyGroup = ELP::createValueHash(shouldBeTransferredFromIIToI);
      auto oldFB_OtherValue = getOtherPolyParam(fb_mix_other, eb)->getControlPositionValue();
      auto valueHashOfSourceUnisonAndMono = ELP::createValueHash(unisonAndMonoII);

      ebUseCases.convertToSingle(VoiceGroup::II);

      THEN("Poly parts were copied from src group to I")
      {
        auto hashOfPolyGroupAfterConvert = ELP::createValueHash(polyParametersI);
        CHECK(hashOfPolyGroup == hashOfPolyGroupAfterConvert);
      }

      THEN("params are defaulted")
      {
        for(auto& vec: {shouldBeDefaultedI, shouldBeDefaultedII})
        {
          for(auto p: vec)
          {
            INFO("should be defaulted " << p->getLongName() << " " << toString(p->getID().getVoiceGroup()) << " but isn't.");
            CHECK(p->isDefaultLoaded());
          }
        }
      }

      THEN("FX from is transfered")
      {
        //TODO implement
      }

      THEN("FB_MIX other II was copied from src to I")
      {
        CHECK(fb_mix_other->getControlPositionValue() == oldFB_OtherValue);
      }

      THEN("FX groups left alone")
      {
        auto totalHashOfAllParametersKeptAsIsAfterConvert = ELP::createHashOfVector(shouldBeKeptAsIs);
        CHECK(totalHashOfAllParametersKeptAsIs == totalHashOfAllParametersKeptAsIsAfterConvert);
      }

      THEN("unison copied from src (II) to -> I")
      {
        auto valueHashOfUnisonAndMonoAfterConvert = ELP::createValueHash(unisonAndMonoI);
        CHECK(valueHashOfSourceUnisonAndMono == valueHashOfUnisonAndMonoAfterConvert);
      }
    }

    WHEN("split-I is converted to Layer") //F2 cont
    {
      auto polyParams = collectParams(eb.findAllParametersOfType({C15::Descriptors::ParameterType::Polyphonic_Unmodulateable, C15::Descriptors::ParameterType::Polyphonic_Modulateable}), eb);
      auto valueHashOfPolyParams = ELP::createValueHash(polyParams);
      ebUseCases.convertToLayer(VoiceGroup::I);

      THEN("poly is kept as is")
      {
        auto afterConvert = ELP::createValueHash(polyParams);
        CHECK(valueHashOfPolyParams == afterConvert);
      }
    }

    WHEN("split-II is converted to Layer") //F2 cont
    {
      auto polyParams = collectParams(eb.findAllParametersOfType({C15::Descriptors::ParameterType::Polyphonic_Unmodulateable, C15::Descriptors::ParameterType::Polyphonic_Modulateable}), eb);
      auto valueHashOfPolyParams = ELP::createValueHash(polyParams);
      ebUseCases.convertToLayer(VoiceGroup::II);

      THEN("poly is kept as is")
      {
        auto afterConvert = ELP::createValueHash(polyParams);
        CHECK(valueHashOfPolyParams == afterConvert);
      }
    }
  }
}