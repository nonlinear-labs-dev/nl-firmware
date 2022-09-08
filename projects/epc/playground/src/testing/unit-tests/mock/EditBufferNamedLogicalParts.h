#pragma once
#include <testing/TestHelper.h>

class EditBufferLogicalParts
{
 private:
  template <typename tC, typename tT> static bool contains(tC c, tT t)
  {
    return std::find(c.begin(), c.end(), t) != c.end();
  }

 public:
  static std::vector<Parameter*> removeElements(const std::vector<Parameter*>& p, std::vector<int> ids)
  {
    std::vector<Parameter*> ret {};
    for(auto& param : p)
    {
      if(std::find(ids.begin(), ids.end(), param->getID().getNumber()) == ids.end())
      {
        ret.emplace_back(param);
      }
    }
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getLocalNormal()
  {
    auto eb = TestHelper::getEditBuffer();
    std::vector<Parameter*> ret {};

    auto ignore = std::vector { "Unison", "Mono", "Part", "Split" };
    auto ignoreParams = std::vector { C15::PID::FB_Mix_Osc,     C15::PID::FB_Mix_Osc_Src, C15::PID::FB_Mix_Comb_Src,
                                      C15::PID::FB_Mix_SVF_Src, C15::PID::FB_Mix_FX_Src,  C15::PID::Out_Mix_To_FX };

    for(auto& g : eb->getParameterGroups(vg))
    {
      if(!contains(ignore, g->getLongName()))
      {
        for(auto& p : g->getParameters())
        {
          if(!contains(ignoreParams, p->getID().getNumber()))
          {
            ret.emplace_back(p);
          }
        }
      }
    }

    return ret;
  }

  std::vector<Parameter*> filter(const std::vector<Parameter*>& in, const std::vector<ParameterId>& exclude)
  {
    std::vector<Parameter*> ret;
    std::copy_if(in.begin(), in.end(), std::back_inserter(ret),
                 [&](Parameter* p)
                 {
                   auto it = std::find(exclude.begin(), exclude.end(), p->getID());
                   return it == exclude.end();
                 });
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getAllNonGlobalParameters()
  {
    auto eb = TestHelper::getEditBuffer();
    std::vector<Parameter*> ret {};
    eb->forEachParameter<vg>([&](Parameter* p) { ret.emplace_back(p); });
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getCrossFB()
  {
    auto eb = TestHelper::getEditBuffer();
    std::vector<Parameter*> ret;
    ret.emplace_back(eb->findParameterByID({ C15::PID::FB_Mix_Comb_Src, vg }));
    ret.emplace_back(eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, vg }));
    ret.emplace_back(eb->findParameterByID({ C15::PID::FB_Mix_Osc, vg }));
    ret.emplace_back(eb->findParameterByID({ C15::PID::FB_Mix_Osc_Src, vg }));
    ret.emplace_back(eb->findParameterByID({ C15::PID::FB_Mix_SVF_Src, vg }));
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getToFX()
  {
    auto eb = TestHelper::getEditBuffer();
    std::vector<Parameter*> ret;
    ret.emplace_back(eb->findParameterByID({ C15::PID::Out_Mix_To_FX, vg }));
    return ret;
  }

  template <VoiceGroup vg> static Parameter* getUnisonVoice()
  {
    return TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Unison_Voices, vg });
  };

  template <VoiceGroup vg> static Parameter* getMonoEnable()
  {
    return TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Mono_Grp_Enable, vg });
  }

  static std::vector<Parameter*> getMaster()
  {
    return { getMasterVolume(), getMasterTune() };
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getVoices()
  {
    std::vector<Parameter*> ret;
    auto unison = getUnison<vg>();
    auto mono = getMono<vg>();
    std::copy(unison.begin(), unison.end(), std::back_inserter(ret));
    std::copy(mono.begin(), mono.end(), std::back_inserter(ret));
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getUnison()
  {
    std::vector<Parameter*> ret;
    for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID(GroupId("Unison", vg))->getParameters())
    {
      ret.emplace_back(p);
    }
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getMono()
  {
    std::vector<Parameter*> ret;
    for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID(GroupId("Mono", vg))->getParameters())
    {
      ret.emplace_back(p);
    }
    return ret;
  }

  template <VoiceGroup vg> static ModulateableParameter* getPartVolume()
  {
    return dynamic_cast<ModulateableParameter*>(
        TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Voice_Grp_Volume, vg }));
  }

  template <VoiceGroup vg> static ModulateableParameter* getPartTune()
  {
    return dynamic_cast<ModulateableParameter*>(
        TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Voice_Grp_Tune, vg }));
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getPartMaster()
  {
    std::vector<Parameter*> ret {};
    ret.emplace_back(getPartVolume<vg>());
    ret.emplace_back(getPartTune<vg>());
    return ret;
  }

  template <VoiceGroup vg> static std::vector<Parameter*> getFade()
  {
    return { getFadeFrom<vg>(), getFadeRange<vg>() };
  }

  template <VoiceGroup vg> static Parameter* getFadeFrom()
  {
    return TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Voice_Grp_Fade_From, vg });
  }

  template <VoiceGroup vg> static Parameter* getFadeRange()
  {
    return TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Voice_Grp_Fade_Range, vg });
  }

  template <VoiceGroup vg> static Parameter* getSplitPoint()
  {
    return TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Split_Split_Point, vg });
  }

  static ModulateableParameter* getMasterVolume()
  {
    return dynamic_cast<ModulateableParameter*>(
        TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global }));
  }

  static ModulateableParameter* getMasterTune()
  {
    return dynamic_cast<ModulateableParameter*>(
        TestHelper::getEditBuffer()->findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global }));
  }

  static std::vector<Parameter*> getModMatrix()
  {
    std::vector<Parameter*> ret {};
    for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID({ "MCM", VoiceGroup::Global })->getParameters())
      ret.emplace_back(p);
    return ret;
  }

  static std::vector<Parameter*> getScale()
  {
    std::vector<Parameter*> ret {};
    for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID({ "Scale", VoiceGroup::Global })->getParameters())
      ret.emplace_back(p);
    return ret;
  }

  static size_t createHashOfVector(const std::vector<Parameter*>& v)
  {
    size_t ret {};
    for(auto& p : v)
    {
      hash_combine(ret, p->getHash());
    }
    return ret;
  }

  static size_t createValueHash(const std::vector<Parameter*>& v)
  {
    size_t ret {};
    for(auto& p : v)
    {
      hash_combine(ret, createValueHash(p));
    }
    return ret;
  }

  static size_t createValueHash(Parameter* p)
  {
    size_t ret;
    hash_combine(ret, p->getValue().getQuantizedClippedValue(true));
    if(auto modP = dynamic_cast<ModulateableParameter*>(p))
    {
      hash_combine(ret, modP->getModulationAmount());
      hash_combine(ret, modP->getModulationSource());
    }
    return ret;
  };

  template <typename... tArgs> static size_t createValueHash(tArgs... args)
  {
    size_t ret {};

    for(auto& v : { args... })
    {
      hash_combine(ret, createValueHash(v));
    }

    return ret;
  }

  template <typename... tArgs> static bool isFactoryDefaultLoaded(tArgs... args)
  {
    for(auto& v : { args... })
    {
      if(!isFactoryDefaultLoaded(v))
        return false;
    }
    return true;
  };

  static bool isFactoryDefaultLoaded(const std::vector<Parameter*>& v)
  {
    for(auto& p : v)
    {
      if(!p->isFactoryDefaultLoaded())
        return false;
    }
    return true;
  }

  static Parameter* getParameter(const ParameterId& id)
  {
    return TestHelper::getEditBuffer()->findParameterByID(id);
  }
};
