#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include <testing/unit-tests/mock/EditBufferNamedLogicalParts.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "all ids of monophonic")
{
  auto& eb = *TestHelper::getEditBuffer();
  auto all = eb.findAllParametersOfType({ C15::Descriptors::ParameterType::Monophonic_Unmodulateable,
                                          C15::Descriptors::ParameterType::Monophonic_Modulateable });
  for(const auto& p : all)
  {
    auto param = eb.findParameterByID(p);
    CHECK(param->isMonophonic());
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "all ids of polyphonic")
{
  auto& eb = *TestHelper::getEditBuffer();
  auto all = eb.findAllParametersOfType({ C15::Descriptors::ParameterType::Polyphonic_Unmodulateable,
                                          C15::Descriptors::ParameterType::Polyphonic_Modulateable });
  for(const auto& p : all)
  {
    auto param = eb.findParameterByID(p);
    CHECK(param->isPolyphonic());
  }
}

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
    for(auto& p : v)
    {
      ret.emplace_back(getOtherPolyParam(p, eb));
    }
    return ret;
  }

  std::vector<Parameter*> collectParams(const std::vector<ParameterId>& ids, EditBuffer& eb)
  {
    std::vector<Parameter*> ret;
    for(auto i : ids)
    {
      ret.emplace_back(eb.findParameterByID(i));
    }
    return ret;
  }

  std::vector<Parameter*> copyIf(const std::vector<Parameter*>& p, std::function<bool(Parameter*)> pred)
  {
    std::vector<Parameter*> ret;
    for(auto& in : p)
    {
      if(pred(in))
        ret.emplace_back(in);
    }
    return ret;
  }

  std::unordered_map<ParameterId, size_t> getHashMap(const std::vector<Parameter*>& p)
  {
    std::unordered_map<ParameterId, size_t> ret;
    for(auto par : p)
    {
      ret[par->getID()] = par->getHash();
    }
    return ret;
  }

  std::unordered_map<int, size_t> getHashMapWithoutVG(const std::vector<Parameter*>& p)
  {
    std::unordered_map<int, size_t> ret;
    for(auto par : p)
    {
      ret[par->getID().getNumber()] = par->getHash();
    }
    return ret;
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert sounds -> general parts")
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

  auto unison_voices_I = eb.findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::I });
  auto unison_detune_I = eb.findParameterByID({ C15::PID::Unison_Detune, VoiceGroup::I });
  auto unison_pan_I = eb.findParameterByID({ C15::PID::Unison_Pan, VoiceGroup::I });
  auto unison_phase_I = eb.findParameterByID({ C15::PID::Unison_Phase, VoiceGroup::I });

  auto mono_enable_I = eb.findParameterByID({ C15::PID::Mono_Grp_Enable, VoiceGroup::I });
  auto mono_enable_II = eb.findParameterByID({ C15::PID::Mono_Grp_Enable, VoiceGroup::II });
  auto mono_glide_I = eb.findParameterByID({ C15::PID::Mono_Grp_Glide, VoiceGroup::I });
  auto mono_legato_I = eb.findParameterByID({ C15::PID::Mono_Grp_Legato, VoiceGroup::I });
  auto mono_prio_I = eb.findParameterByID({ C15::PID::Mono_Grp_Prio, VoiceGroup::I });

  auto part_vol_I = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto part_tune_I = eb.findParameterByID({ C15::PID::Voice_Grp_Tune, VoiceGroup::I });

  WHEN("split is loaded")
  {
    ebUseCases.initSoundAs(SoundType::Split, Defaults::FactoryDefault);

    WHEN("convert II to single")
    {
      auto to_fx = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
      auto to_fx_II = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
      auto fx_from = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
      auto fx_from_II = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });

      {
        auto testScope = TestHelper::createTestScope();
        to_fx_II->setCPFromHwui(testScope->getTransaction(), 0.3);
        fx_from_II->setCPFromHwui(testScope->getTransaction(), 0.3);
      }

      auto to_fx_II_pre = to_fx_II->getControlPositionValue();
      auto to_fx_I_pre = to_fx->getControlPositionValue();
      auto fx_from_I_pre = fx_from->getControlPositionValue();
      auto fx_from_II_pre = fx_from_II->getControlPositionValue();

      WHEN("convert I")
      {
        ebUseCases.convertToSingle(VoiceGroup::I);

        THEN("to-fx not inverted")
        {
          CHECK(to_fx->getControlPositionValue() == Approx(to_fx_I_pre));
        }

        THEN("fx_from not inverted")
        {
          CHECK(fx_from->getControlPositionValue() == Approx(fx_from_I_pre));
        }
      }

      WHEN("convert II")
      {
        ebUseCases.convertToSingle(VoiceGroup::II);

        THEN("to-fx inverted")
        {
          CHECK(to_fx->getControlPositionValue() == Approx(1 - to_fx_II_pre));
        }

        THEN("fx_from inverted")
        {
          CHECK(fx_from->getControlPositionValue() == Approx(1 - fx_from_II_pre));
        }
      }
    }
  }

  WHEN("layer is loaded")
  {
    ebUseCases.initSoundAs(SoundType::Layer, Defaults::FactoryDefault);

    WHEN("convert II to single")
    {
      auto to_fx = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
      auto to_fx_II = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
      auto fx_from = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
      auto fx_from_II = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });

      {
        auto testScope = TestHelper::createTestScope();
        to_fx_II->setCPFromHwui(testScope->getTransaction(), 0.3);
        fx_from_II->setCPFromHwui(testScope->getTransaction(), 0.3);
      }

      auto to_fx_II_pre = to_fx_II->getControlPositionValue();
      auto to_fx_I_pre = to_fx->getControlPositionValue();
      auto fx_from_I_pre = fx_from->getControlPositionValue();
      auto fx_from_II_pre = fx_from_II->getControlPositionValue();

      WHEN("convert I")
      {
        ebUseCases.convertToSingle(VoiceGroup::I);

        THEN("to-fx not inverted")
        {
          CHECK(to_fx->getControlPositionValue() == Approx(to_fx_I_pre));
        }

        THEN("fx_from not inverted")
        {
          CHECK(fx_from->getControlPositionValue() == Approx(fx_from_I_pre));
        }
      }

      WHEN("convert II")
      {
        ebUseCases.convertToSingle(VoiceGroup::II);

        THEN("to-fx inverted")
        {
          CHECK(to_fx->getControlPositionValue() == Approx(1 - to_fx_II_pre));
        }

        THEN("fx_from inverted")
        {
          CHECK(fx_from->getControlPositionValue() == Approx(1 - fx_from_II_pre));
        }
      }
    }
  }

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
  }

  WHEN("Split preset is prepared and loaded")
  {
    auto split = presets.getSplitPreset();
    ebUseCases.load(split);
    ebUseCases.randomize(0.1);

    ParameterUseCases voicesUseCasesI(voicesI);
    ParameterUseCases voicesUseCasesII(voicesII);
    ParameterUseCases monoUseCasesI(mono_enable_I);
    ParameterUseCases monoUseCasesII(mono_enable_II);

    voicesUseCasesI.setControlPosition(0.25);
    voicesUseCasesII.setControlPosition(0.25 + (1. / 12.));

    monoUseCasesI.setControlPosition(1);
    monoUseCasesII.setControlPosition(0);

    REQUIRE(voicesI->getDisplayString() == "4 voices");
    REQUIRE(voicesII->getDisplayString() == "5 voices");

    REQUIRE(mono_enable_I->getDisplayString() == "On");
    REQUIRE(mono_enable_II->getDisplayString() == "Off");

    WHEN("convert I to single")
    {
      std::vector<Parameter*> shouldBeTransferredFromSourceToI = { fb_mix_other, out_mix_to_other, mono_some };
      std::vector<Parameter*> shouldBeDefaultedI
          = { fb_mix_osc, fb_mix_osc_src, fb_mix_svf_from, split_I, fade_rng_I, fade_from_I, part_tune_I, part_vol_I };
      std::vector<Parameter*> shouldBeDefaultedII = getOtherPolyVector(shouldBeDefaultedI, eb);

      std::vector<Parameter*> unisonAndMonoI = { unison_detune_I, unison_pan_I,  unison_phase_I, mono_enable_I,
                                                 mono_glide_I,    mono_legato_I, mono_prio_I };  //without voices

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
        for(auto& vec : { shouldBeDefaultedI, shouldBeDefaultedII })
        {
          for(auto p : vec)
          {
            INFO("should be defaulted " << p->getLongName() << " " << toString(p->getID().getVoiceGroup())
                                        << " but isn't.");
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
      std::vector<Parameter*> polyParametersI = { fb_mix_other, out_mix_to_other, mono_some };
      std::vector<Parameter*> shouldBeTransferredFromIIToI = getOtherPolyVector(polyParametersI, eb);
      std::vector<Parameter*> shouldBeDefaultedI
          = { fb_mix_osc, fb_mix_osc_src, fb_mix_svf_from, split_I, fade_rng_I, fade_from_I };
      std::vector<Parameter*> shouldBeDefaultedII = getOtherPolyVector(shouldBeDefaultedI, eb);
      std::vector<Parameter*> unisonAndMonoI = { unison_detune_I, unison_pan_I,  unison_phase_I, mono_enable_I,
                                                 mono_glide_I,    mono_legato_I, mono_prio_I };  //without voices!!!
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
        for(auto& vec : { shouldBeDefaultedI, shouldBeDefaultedII })
        {
          for(auto p : vec)
          {
            INFO("should be defaulted " << p->getLongName() << " " << toString(p->getID().getVoiceGroup())
                                        << " but isn't.");
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

    using tParVec = std::vector<Parameter*>;

    //TODO add FX_MIX to this vec
    auto shouldBeDefaultedSplitToLayer
        = tParVec { split_I, split_II, fade_rng_I, fade_rng_II, fade_from_I, fade_from_II };

    WHEN("split-I is converted to Layer")
    {
      auto polyParams
          = collectParams(eb.findAllParametersOfType({ C15::Descriptors::ParameterType::Polyphonic_Unmodulateable,
                                                       C15::Descriptors::ParameterType::Polyphonic_Modulateable }),
                          eb);

      polyParams = copyIf(polyParams, [](Parameter* p) {
        using namespace C15::PID;
        auto id = p->getID();
        auto pid = id.getNumber();
        return pid != Voice_Grp_Fade_From && pid != Voice_Grp_Fade_Range && pid != Unison_Voices
            && pid != Split_Split_Point && pid != Unison_Detune && pid != Unison_Phase && pid != Unison_Pan
            && pid != Mono_Grp_Glide && pid != Mono_Grp_Enable && pid != Mono_Grp_Prio && pid != Mono_Grp_Legato;
      });

      polyParams = copyIf(polyParams, [](Parameter* p) {
        using namespace C15::PID;
        auto id = p->getID();
        auto pid = id.getNumber();
        return pid != FB_Mix_Comb_Src && pid != FB_Mix_SVF_Src;
      });

      auto valueHashOfPolyParams = ELP::createValueHash(polyParams);
      auto monophonicParam
          = tParVec { flanger_other_I, flanger_other_II, reverb_other_I, reverb_other_II, gap_other_I, gap_other_II };
      auto otherPolyParams = tParVec { fb_mix_fx_from, fb_mix_other, out_mix_to_other };
      auto valueHashOfMonoPhonicParams = ELP::createValueHash(monophonicParam);
      auto valueHashOfOtherPolyParams = ELP::createValueHash(otherPolyParams);
      auto hashMapOfValuesOfPolyParams = getHashMap(polyParams);

      auto partVolumes
          = tParVec { part_vol_I, part_tune_I, getOtherPolyParam(part_vol_I, eb), getOtherPolyParam(part_tune_I, eb) };
      auto valueHashOfPartVolumes = ELP::createValueHash(partVolumes);

      ebUseCases.convertToLayer(VoiceGroup::I);

      THEN("poly is kept as is")
      {
        for(auto p : polyParams)
        {
          INFO(p->getParentGroup()->getLongName() << " - " << p->getLongName() << " not same hash as before!");
          CHECK(hashMapOfValuesOfPolyParams[p->getID()] == p->getHash());
        }

        auto afterConvert = ELP::createValueHash(polyParams);
        CHECK(valueHashOfPolyParams == afterConvert);
      }

      THEN("part volumes kept as is")
      {
        auto afterConvert = ELP::createValueHash(partVolumes);
        CHECK(afterConvert == valueHashOfPartVolumes);
      }

      THEN("monos is kept as is")
      {
        auto afterConvertMono = ELP::createValueHash(monophonicParam);
        auto afterConvertPoly = ELP::createValueHash(otherPolyParams);
        CHECK(valueHashOfMonoPhonicParams == afterConvertMono);
        CHECK(valueHashOfOtherPolyParams == afterConvertPoly);
      }

      THEN("FB-Params defaulted")
      {
        std::vector<Parameter*> shouldBeDefaulted = { fb_mix_osc, fb_mix_comb_from, fb_mix_svf_from };
        for(auto d : shouldBeDefaulted)
        {
          INFO("parameter: " << d->getLongName() << " is not default");
          CHECK(d->isDefaultLoaded());
        }
      }

      THEN("Master and Part Params defaulted")
      {
        for(auto p : shouldBeDefaultedSplitToLayer)
        {
          INFO("Parameter: " << p->getLongName() << " is not default");
          CHECK(p->isDefaultLoaded());
        }
      }

      THEN("Unsion Voices Correct")
      {
        CHECK(voicesI->getDisplayString() == "4 voices");
        CHECK(mono_enable_I->getDisplayString() == "On");
      }
    }

    WHEN("split-II is converted to Layer")
    {
      auto polyParams
          = collectParams(eb.findAllParametersOfType({ C15::Descriptors::ParameterType::Polyphonic_Unmodulateable,
                                                       C15::Descriptors::ParameterType::Polyphonic_Modulateable }),
                          eb);

      polyParams = copyIf(polyParams, [](Parameter* p) {
        using namespace C15::PID;
        auto id = p->getID();
        auto pid = id.getNumber();
        return pid != Voice_Grp_Fade_From && pid != Voice_Grp_Fade_Range && pid != Unison_Voices
            && pid != Split_Split_Point && pid != Unison_Detune && pid != Unison_Phase && pid != Unison_Pan
            && pid != Mono_Grp_Glide && pid != Mono_Grp_Enable && pid != Mono_Grp_Prio && pid != Mono_Grp_Legato;
      });

      polyParams = copyIf(polyParams, [](Parameter* p) {
        using namespace C15::PID;
        auto id = p->getID();
        auto pid = id.getNumber();
        return pid != FB_Mix_Comb_Src && pid != FB_Mix_SVF_Src;
      });

      auto hashMapOfValuesOfPolyParams = getHashMap(polyParams);

      auto valueHashOfPolyParams = ELP::createValueHash(polyParams);
      auto monophonicParam
          = tParVec { flanger_other_I, flanger_other_II, reverb_other_I, reverb_other_II, gap_other_I, gap_other_II };
      auto otherPolyParams = tParVec { fb_mix_fx_from, fb_mix_other, out_mix_to_other };
      auto valueHashOfMonoPhonicParams = ELP::createValueHash(monophonicParam);
      auto valueHashOfOtherPolyParams = ELP::createValueHash(otherPolyParams);
      auto partVolumes
          = tParVec { part_vol_I, part_tune_I, getOtherPolyParam(part_vol_I, eb), getOtherPolyParam(part_tune_I, eb) };
      auto valueHashOfPartVolumes = ELP::createValueHash(partVolumes);

      ebUseCases.convertToLayer(VoiceGroup::II);

      THEN("poly is kept as is")
      {
        for(auto p : polyParams)
        {
          INFO(p->getParentGroup()->getLongName() << " - " << p->getLongName() << " not same hash as before!");
          CHECK(hashMapOfValuesOfPolyParams[p->getID()] == p->getHash());
        }

        auto afterConvert = ELP::createValueHash(polyParams);
        CHECK(valueHashOfPolyParams == afterConvert);
      }

      THEN("part volumes kept as is")
      {
        auto afterConvert = ELP::createValueHash(partVolumes);
        CHECK(afterConvert == valueHashOfPartVolumes);
      }

      THEN("monos is kept as is")
      {
        auto afterConvertMono = ELP::createValueHash(monophonicParam);
        auto afterConvertPoly = ELP::createValueHash(otherPolyParams);
        CHECK(valueHashOfMonoPhonicParams == afterConvertMono);
        CHECK(valueHashOfOtherPolyParams == afterConvertPoly);
      }

      THEN("FB-Params defaulted")
      {
        std::vector<Parameter*> shouldBeDefaulted = { fb_mix_osc, fb_mix_comb_from, fb_mix_svf_from };
        for(auto d : shouldBeDefaulted)
        {
          INFO("parameter: " << d->getLongName() << " is not default");
          CHECK(d->isDefaultLoaded());
        }
      }

      THEN("Master and Part Params defaulted")
      {
        for(auto p : shouldBeDefaultedSplitToLayer)
        {
          INFO("Parameter: " << p->getLongName() << " is not default");
          CHECK(p->isDefaultLoaded());
        }
      }

      THEN("Unsion Voices Correct")
      {
        CHECK(voicesI->getDisplayString() == "5 voices");
        CHECK(mono_enable_I->getDisplayString() == "Off");
      }
    }
  }

  WHEN("Layer preset is prepared and loaded")
  {
    std::vector<Parameter*> shouldBeDefaulted
        = { fb_mix_osc, fb_mix_osc_src, fb_mix_comb_from, fb_mix_svf_from, split_I,
            split_II,   fade_from_I,    fade_from_II,     fade_rng_I,      fade_rng_II };

    auto allPolys
        = collectParams(eb.findAllParametersOfType({ C15::Descriptors::ParameterType::Polyphonic_Modulateable,
                                                     C15::Descriptors::ParameterType::Polyphonic_Unmodulateable }),
                        eb);

    auto layer = presets.getLayerPreset();
    ebUseCases.load(layer);
    ebUseCases.randomize(0.1);

    auto disallowedIDs = [](Parameter* p) {
      using namespace C15::PID;
      auto id = p->getID();
      auto pid = id.getNumber();
      return pid != Voice_Grp_Fade_From && pid != Voice_Grp_Fade_Range && pid != Unison_Voices
          && pid != Split_Split_Point && pid != Unison_Detune && pid != Unison_Phase && pid != Unison_Pan
          && pid != Mono_Grp_Glide && pid != Mono_Grp_Enable && pid != Mono_Grp_Prio && pid != Mono_Grp_Legato
          && pid != FB_Mix_Comb_Src && pid != FB_Mix_SVF_Src && pid != FB_Mix_Osc_Src && pid != FB_Mix_FX_Src
          && pid != FB_Mix_Osc && pid != Out_Mix_To_FX && pid != Voice_Grp_Volume && pid != Voice_Grp_Tune;
    };

    WHEN("convert I to single")
    {
      auto polysOfSource = copyIf(allPolys, [](Parameter* p) { return p->getVoiceGroup() == VoiceGroup::I; });

      polysOfSource = copyIf(polysOfSource, disallowedIDs);

      auto polyHashOfSource = ELP::createValueHash(polysOfSource);
      auto polyHashes = getHashMap(polysOfSource);

      auto fb_mix_other_value = ELP::createValueHash(fb_mix_other);

      ebUseCases.convertToSingle(VoiceGroup::I);

      THEN("Polys were copied to I")
      {
        for(auto p : polysOfSource)
        {
          INFO(p->getParentGroup()->getLongName() << "-" << p->getLongName() << " was changed!");
          CHECK(polyHashes[p->getID()] == p->getHash());
        }
        CHECK(ELP::createValueHash(polysOfSource) == polyHashOfSource);
      }

      THEN("defaulted")
      {
        for(auto p : shouldBeDefaulted)
        {
          INFO(p->getLongName() << " is not defaulted");
          CHECK(p->isDefaultLoaded());
        }
      }
    }

    WHEN("convert II to single")
    {
      auto polysOfSource = copyIf(allPolys, [](Parameter* p) { return p->getVoiceGroup() == VoiceGroup::II; });
      auto polysTarget = copyIf(allPolys, [](Parameter* p) { return p->getVoiceGroup() == VoiceGroup::I; });

      polysOfSource = copyIf(polysOfSource, disallowedIDs);
      polysTarget = copyIf(polysTarget, disallowedIDs);

      auto polyHashMap = getHashMapWithoutVG(polysOfSource);

      auto polyHashOfSource = ELP::createValueHash(polysOfSource);

      auto fb_mix_other_value = ELP::createValueHash(getOtherPolyParam(fb_mix_other, eb));

      ebUseCases.convertToSingle(VoiceGroup::II);

      THEN("Polys were copied to I")
      {
        for(auto p : polysTarget)
        {
          CHECK(polyHashMap[p->getID().getNumber()] == p->getHash());
        }
        CHECK(ELP::createValueHash(polysTarget) == polyHashOfSource);
      }

      THEN("defaulted")
      {
        for(auto p : shouldBeDefaulted)
        {
          INFO(p->getLongName() << " is not defaulted");
          CHECK(p->isDefaultLoaded());
        }
      }
    }
  }
}
//TODO continue with layer to split!

//not very smart to use the same algorithm as in the editbuffer?
namespace
{
  double parabolicFadeCpToAmplitude(const double cp)
  {
    return 1.0 - (cp * cp);
  }

  double amplitudeToParabolicGainCp(const double af)
  {
    return std::sqrt(af) * 0.5;
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert sounds -> new parameters -> explicit")
{
  auto& eb = *TestHelper::getEditBuffer();
  auto masterParameter = eb.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto fx_mixParameter = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global });
  auto partVolI = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto partVolII = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::II });

  MockPresetStorage presets;
  EditBufferUseCases ebUseCases(eb);

  WHEN("Single loaded")
  {
    ebUseCases.load(presets.getSinglePreset());

    ebUseCases.initSound(Defaults::FactoryDefault);

    WHEN("FX Mix to 0% and master volume is set to 0 dB")
    {
      auto fx_mix_pos = 0.0;
      auto mst_vol = 0.5;

      ParameterUseCases masterVol(masterParameter);
      masterVol.setControlPosition(mst_vol);
      ParameterUseCases fxMix(fx_mixParameter);
      fxMix.setControlPosition(fx_mix_pos);

      REQUIRE(masterParameter->getDisplayString() == "0.000 dB");
      REQUIRE(fx_mixParameter->getDisplayString() == "0.0 %");

      WHEN("convert to layer")
      {
        ebUseCases.convertToLayer(VoiceGroup::I);

        INFO("with master_vol: " << mst_vol << " fx_mix_pos: " << fx_mix_pos);
        CHECK(partVolI->getDisplayString() == "0.000 dB");
        CHECK(partVolII->getDisplayString() == "-inf dB");
      }

      WHEN("convert to split")
      {
        ebUseCases.convertToSplit(VoiceGroup::I);

        INFO("with master_vol: " << mst_vol << " fx_mix_pos: " << fx_mix_pos);
        CHECK(partVolI->getDisplayString() == "0.000 dB");
        CHECK(partVolII->getDisplayString() == "-inf dB");

        THEN("convert back")
        {
          ebUseCases.convertToSingle(VoiceGroup::I);

          REQUIRE(masterParameter->getDisplayString() == "0.000 dB");
          REQUIRE(fx_mixParameter->getDisplayString() == "0.0 %");
        }
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert sounds -> new parameters -> explicit 2")
{
  auto& eb = *TestHelper::getEditBuffer();
  auto masterParameter = eb.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto fx_mixParameter = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global });
  auto partVolI = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto partVolII = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::II });

  MockPresetStorage presets;
  EditBufferUseCases ebUseCases(eb);

  WHEN("Single loaded")
  {
    ebUseCases.load(presets.getSinglePreset());

    ebUseCases.initSound(Defaults::FactoryDefault);

    WHEN("FX Mix to 50% and master volume is set to 0 dB")
    {
      auto fx_mix_pos = 0.5;
      auto mst_vol = 0.5;

      ParameterUseCases masterVol(masterParameter);
      masterVol.setControlPosition(mst_vol);
      ParameterUseCases fxMix(fx_mixParameter);
      fxMix.setControlPosition(fx_mix_pos);

      REQUIRE(masterParameter->getDisplayString() == "0.000 dB");
      REQUIRE(fx_mixParameter->getDisplayString() == "50.0 %");

      WHEN("convert to layer")
      {
        ebUseCases.convertToLayer(VoiceGroup::I);

        INFO("with master_vol: " << mst_vol << " fx_mix_pos: " << fx_mix_pos);
        CHECK(partVolI->getControlPositionValue() == partVolI->getValue().getQuantizedValue(0.433, true));

        CHECK(partVolI->getDisplayString() == "-2.50 dB");
        CHECK(partVolII->getDisplayString() == "-2.50 dB");

        THEN("convert back")
        {
          ebUseCases.convertToSingle(VoiceGroup::I);

          REQUIRE(masterParameter->getDisplayString() == "0.000 dB");
          REQUIRE(fx_mixParameter->getDisplayString() == "50.0 %");
        }
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert sounds -> new parameters -> explicit 3")
{
  auto& eb = *TestHelper::getEditBuffer();
  auto masterParameter = eb.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto fx_mixParameter = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global });
  auto partVolI = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto partVolII = eb.findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::II });

  MockPresetStorage presets;
  EditBufferUseCases ebUseCases(eb);

  WHEN("Split loaded")
  {
    ebUseCases.load(presets.getSplitPreset());

    ebUseCases.initSound(Defaults::FactoryDefault);

    WHEN("FX Mix to 50% and master volume is set to 0 dB")
    {
      auto mst_vol = 0.5;
      auto part_vol = 0.5;

      ParameterUseCases masterVol(masterParameter);
      masterVol.setControlPosition(mst_vol);

      ParameterUseCases partIVol(partVolI);
      ParameterUseCases partIIVol(partVolII);

      partIVol.setControlPosition(part_vol);
      partIIVol.setControlPosition(part_vol);

      REQUIRE(masterParameter->getDisplayString() == "0.000 dB");
      REQUIRE(partVolI->getDisplayString() == "0.000 dB");
      REQUIRE(partVolII->getDisplayString() == "0.000 dB");

      WHEN("convert to single")
      {
        ebUseCases.convertToSingle(VoiceGroup::I);

        INFO("with master_vol: " << mst_vol << " and part-vol " << part_vol);
        CHECK(masterParameter->getControlPositionValue() == Approx(0.577));
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "convert split to single: in poly landet part I selbst bei selektion von II")
{
  MockPresetStorage presets;
  presets.getSplitPreset();

  auto& eb = *TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(eb);
  ebUseCases.randomize(0.2);

  auto num = GENERATE(C15::PID::Env_C_Loop, C15::PID::Out_Mix_A_Lvl, C15::PID::FB_Mix_Comb);

  WHEN("parameter " << num << " changed")
  {
    auto some_poly_param_I = eb.findParameterByID({ num, VoiceGroup::I });
    auto some_poly_param_II = eb.findParameterByID({ num, VoiceGroup::II });

    ParameterUseCases pI(some_poly_param_I);
    ParameterUseCases pII(some_poly_param_II);

    pI.setControlPosition(0);
    pII.setControlPosition(1);

    WHEN("convert I to single")
    {
      ebUseCases.convertToSingle(VoiceGroup::I);
      CHECK(some_poly_param_I->getControlPositionValue() == 0);
    }

    WHEN("convert II to single")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(some_poly_param_I->getControlPositionValue() == 1);
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert Single to dual: FB Mix FX From")
{
  MockPresetStorage presets;
  auto& eb = *TestHelper::getEditBuffer();

  auto parameterI = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto parameterII = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });

  TestHelper::initSingleEditBuffer();

  EditBufferUseCases ebUseCases(eb);

  auto CP = GENERATE(0.0, 0.4, 0.5, 0.7, 1.0);

  WHEN("Single init with " << CP)
  {
    auto evII = 1 - CP;
    ParameterUseCases pI(parameterI);
    pI.setControlPosition(CP);

    WHEN("converted I to split")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }

    WHEN("converted II to split")
    {
      ebUseCases.convertToSplit(VoiceGroup::II);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }

    WHEN("converted I to layer")
    {
      ebUseCases.convertToLayer(VoiceGroup::I);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }

    WHEN("converted II to Layer")
    {
      ebUseCases.convertToLayer(VoiceGroup::II);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert Single to dual with fx I: FB Mix FX From and Out_mix::to_fx")
{
  MockPresetStorage presets;
  auto& eb = *TestHelper::getEditBuffer();

  auto parameterI = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto parameterII = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });
  auto to_fx_I = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto to_fx_II = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });

  TestHelper::initSingleEditBuffer();

  EditBufferUseCases ebUseCases(eb);

  auto CP = GENERATE(0.0, 0.4, 0.5, 0.7, 1.0);

  WHEN("Single init with " << CP)
  {
    ParameterUseCases pI(parameterI);
    pI.setControlPosition(CP);
    ParameterUseCases p2I(to_fx_I);
    p2I.setControlPosition(CP);

    WHEN("converted to split")
    {
      ebUseCases.convertSingleToSplitFXIOnly();
      CHECK(parameterI->isFactoryDefaultLoaded());
      CHECK(parameterII->isFactoryDefaultLoaded());
      CHECK(to_fx_I->isFactoryDefaultLoaded());
      CHECK(to_fx_II->isFactoryDefaultLoaded());
    }

    WHEN("converted to layer")
    {
      ebUseCases.convertSingleToLayerFXIOnly();
      CHECK(parameterI->isFactoryDefaultLoaded());
      CHECK(parameterII->isFactoryDefaultLoaded());
      CHECK(to_fx_I->isFactoryDefaultLoaded());
      CHECK(to_fx_II->isFactoryDefaultLoaded());
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert Single to dual: Out Mix To FX")
{
  MockPresetStorage presets;
  auto& eb = *TestHelper::getEditBuffer();

  auto parameterI = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto parameterII = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });

  TestHelper::initSingleEditBuffer();

  EditBufferUseCases ebUseCases(eb);

  auto CP = GENERATE(0.0, 0.4, 0.5, 0.7, 1.0);

  WHEN("Single init with " << CP)
  {
    auto evII = 1 - CP;
    ParameterUseCases pI(parameterI);
    pI.setControlPosition(CP);

    WHEN("converted I to split")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }

    WHEN("converted II to split")
    {
      ebUseCases.convertToSplit(VoiceGroup::II);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }

    WHEN("converted I to layer")
    {
      ebUseCases.convertToLayer(VoiceGroup::I);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }

    WHEN("converted II to Layer")
    {
      ebUseCases.convertToLayer(VoiceGroup::II);
      CHECK(parameterI->getControlPositionValue() == Approx(CP));
      CHECK(parameterII->getControlPositionValue() == Approx(evII));
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "Converting a Single Sound to Split/Layer should not reset Master Serial FX and FX Pan")
{
  auto eb = TestHelper::getEditBuffer();

  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.convertToSingle(VoiceGroup::I);
  ebUseCases.initSound(Defaults::FactoryDefault);

  WHEN("master serial fx is set to non default")
  {
    auto id = C15::PID::Master_Serial_FX;
    auto p = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases pUseCases(p);
    pUseCases.setControlPosition(0.187);
    REQUIRE_FALSE(p->isDefaultLoaded());

    THEN("converted to split")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      REQUIRE(p->getControlPositionValue() == Approx(0.187));
    }

    THEN("converted to layer")
    {
      ebUseCases.convertToLayer(VoiceGroup::I);
      REQUIRE(p->getControlPositionValue() == Approx(0.187));
    }
  }

  WHEN("master fx pan is set to non default")
  {
    auto id = C15::PID::Master_Pan;
    auto p = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases pUseCases(p);
    pUseCases.setControlPosition(0.187);
    REQUIRE_FALSE(p->isDefaultLoaded());

    THEN("converted to split")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      REQUIRE(p->getControlPositionValue() == Approx(0.187));
    }

    THEN("converted to layer")
    {
      ebUseCases.convertToLayer(VoiceGroup::I);
      REQUIRE(p->getControlPositionValue() == Approx(0.187));
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "Converting a Single Sound to Split/Layer with FX-I only should reset Master Serial FX and FX Pan")
{
  auto eb = TestHelper::getEditBuffer();

  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.convertToSingle(VoiceGroup::I);
  ebUseCases.initSound(Defaults::FactoryDefault);

  WHEN("master serial fx is set to non default")
  {
    auto id = C15::PID::Master_Serial_FX;
    auto p = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases pUseCases(p);
    pUseCases.setControlPosition(0.187);
    REQUIRE_FALSE(p->isDefaultLoaded());

    THEN("converted to split")
    {
      ebUseCases.convertSingleToSplitFXIOnly();
      REQUIRE(p->getControlPositionValue() == Approx(0));
    }

    THEN("converted to layer")
    {
      ebUseCases.convertSingleToLayerFXIOnly();
      REQUIRE(p->getControlPositionValue() == Approx(0));
    }
  }

  WHEN("master fx pan is set to non default")
  {
    auto id = C15::PID::Master_Pan;
    auto p = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases pUseCases(p);
    pUseCases.setControlPosition(0.187);
    REQUIRE_FALSE(p->isDefaultLoaded());

    THEN("converted to split")
    {
      ebUseCases.convertSingleToSplitFXIOnly();
      REQUIRE(p->getControlPositionValue() == Approx(0));
    }

    THEN("converted to layer")
    {
      ebUseCases.convertSingleToLayerFXIOnly();
      REQUIRE(p->getControlPositionValue() == Approx(0));
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,
                 "Converting a Dual Sound to Single should not reset Master Serial FX and FX Pan")
{
  auto eb = TestHelper::getEditBuffer();

  EditBufferUseCases ebUseCases(*eb);
  auto fromType = GENERATE(SoundType::Layer, SoundType::Split);
  auto useVG = GENERATE(VoiceGroup::I, VoiceGroup::II);
  ebUseCases.initSoundAs(fromType, Defaults::FactoryDefault);

  WHEN("master serial fx is set to non default")
  {
    auto id = C15::PID::Master_Serial_FX;
    auto p = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases pUseCases(p);
    pUseCases.setControlPosition(0.187);
    REQUIRE_FALSE(p->isDefaultLoaded());

    THEN("converted " << toString(fromType) << " - " << toString(useVG) << " to single")
    {
      ebUseCases.convertToSingle(VoiceGroup::I);
      REQUIRE(p->getControlPositionValue() == Approx(0.187));
    }
  }

  WHEN("master fx pan is set to non default")
  {
    auto id = C15::PID::Master_Pan;
    auto p = eb->findParameterByID({ id, VoiceGroup::Global });
    ParameterUseCases pUseCases(p);
    pUseCases.setControlPosition(0.187);
    REQUIRE_FALSE(p->isDefaultLoaded());

    THEN("converted " << toString(fromType) << " - " << toString(useVG) << " to single")
    {
      ebUseCases.convertToSingle(VoiceGroup::I);
      REQUIRE(p->getControlPositionValue() == Approx(0.187));
    }
  }
}