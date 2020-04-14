#pragma once

class EditBufferLogicalParts
{
 public:
  template <VoiceGroup vg> static std::vector<Parameter*> getLocalNormal();

  template <VoiceGroup vg> static std::vector<Parameter*> getLocalSpecial();

  template <VoiceGroup vg> static Parameter* getUnisonVoice();

  template <VoiceGroup vg> static Parameter* getMonoEnable();

  template <VoiceGroup vg> static std::vector<Parameter*> getUnison();

  template <VoiceGroup vg> static std::vector<Parameter*> getMono();

  template <VoiceGroup vg> static ModulateableParameter* getPartVolume();

  template <VoiceGroup vg> static ModulateableParameter* getPartTune();

  template <VoiceGroup vg> static std::vector<Parameter*> getPartMaster();

  template <VoiceGroup vg> static Parameter* getFadeFrom();

  template <VoiceGroup vg> static Parameter* getFadeRange();

  static Parameter* getSplitPoint();

  static ModulateableParameter* getMasterVolume();
  static ModulateableParameter* getMasterTune();

  static std::vector<Parameter*> getModMatrix();

  static std::vector<Parameter*> getScale();

  static size_t createHashOfVector(const std::vector<Parameter*>& v);

  static Parameter* getParameter(const ParameterId& id);
};

template <typename tC, typename tT> bool contains(tC c, tT t)
{
  return std::find(c.begin(), c.end(), t) != c.end();
}

namespace detail
{
  constexpr static auto MONO_ENABLE = 364;

  constexpr static auto MASTER_VOLUME = 247;
  constexpr static auto MASTER_TUNE = 248;

  constexpr static auto PART_VOLUME = 358;
  constexpr static auto PART_TUNE = 360;
  constexpr static auto FADE_FROM = 396;
  constexpr static auto FADE_RANGE = 397;

  constexpr static auto SPLIT_POINT = 356;

  constexpr static auto UNISON_VOICES = 249;

  constexpr static auto FB_OSC = 346;
  constexpr static auto FB_OSC_SRC = 348;
  constexpr static auto FB_COMB_FROM = 350;
  constexpr static auto FB_SVF_FROM = 352;
  constexpr static auto FB_FX_FROM = 354;
  constexpr static auto OUT_TO_FX = 362;
}

template <VoiceGroup vg> std::vector<Parameter*> EditBufferLogicalParts::getLocalNormal()
{
  using namespace detail;
  auto eb = TestHelper::getEditBuffer();
  std::vector<Parameter*> ret {};

  auto ignore = std::vector { "Unison", "Mono", "Part" };
  auto ignoreParams = std::vector { FB_OSC, FB_OSC_SRC, FB_COMB_FROM, FB_SVF_FROM, FB_FX_FROM, OUT_TO_FX };

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

template <VoiceGroup vg> std::vector<Parameter*> EditBufferLogicalParts::getLocalSpecial()
{
  using namespace detail;
  auto eb = TestHelper::getEditBuffer();
  std::vector<Parameter*> ret {};

  auto ignoreParams = std::vector { FB_OSC, FB_OSC_SRC, FB_COMB_FROM, FB_SVF_FROM, FB_FX_FROM, OUT_TO_FX };

  for(auto& g : eb->getParameterGroups(vg))
  {
    for(auto& p : g->getParameters())
    {
      if(contains(ignoreParams, p->getID().getNumber()))
      {
        ret.emplace_back(p);
      }
    }
  }

  return ret;
}

template <VoiceGroup vg> Parameter* EditBufferLogicalParts::getUnisonVoice()
{
  return TestHelper::getEditBuffer()->findParameterByID({ detail::UNISON_VOICES, vg });
}

template <VoiceGroup vg> std::vector<Parameter*> EditBufferLogicalParts::getUnison()
{
  std::vector<Parameter*> ret;
  for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID(GroupId("Unison", vg))->getParameters())
  {
    ret.emplace_back(p);
  }
  return ret;
}

template <VoiceGroup vg> std::vector<Parameter*> EditBufferLogicalParts::getMono()
{
  std::vector<Parameter*> ret;
  for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID(GroupId("Mono", vg))->getParameters())
  {
    ret.emplace_back(p);
  }
  return ret;
}

template <VoiceGroup vg> ModulateableParameter* EditBufferLogicalParts::getPartVolume()
{
  return dynamic_cast<ModulateableParameter*>(
      TestHelper::getEditBuffer()->findParameterByID({ detail::PART_VOLUME, vg }));
}

template <VoiceGroup vg> ModulateableParameter* EditBufferLogicalParts::getPartTune()
{
  return dynamic_cast<ModulateableParameter*>(
      TestHelper::getEditBuffer()->findParameterByID({ detail::PART_TUNE, vg }));
}

template <VoiceGroup vg> Parameter* EditBufferLogicalParts::getFadeFrom()
{
  return TestHelper::getEditBuffer()->findParameterByID({ detail::FADE_FROM, vg });
}

template <VoiceGroup vg> Parameter* EditBufferLogicalParts::getFadeRange()
{
  return TestHelper::getEditBuffer()->findParameterByID({ detail::FADE_RANGE, vg });
}

Parameter* EditBufferLogicalParts::getSplitPoint()
{
  return TestHelper::getEditBuffer()->findParameterByID({ detail::SPLIT_POINT, VoiceGroup::Global });
}

ModulateableParameter* EditBufferLogicalParts::getMasterVolume()
{
  return dynamic_cast<ModulateableParameter*>(
      TestHelper::getEditBuffer()->findParameterByID({ detail::MASTER_VOLUME, VoiceGroup::Global }));
}

ModulateableParameter* EditBufferLogicalParts::getMasterTune()
{
  return dynamic_cast<ModulateableParameter*>(
      TestHelper::getEditBuffer()->findParameterByID({ detail::MASTER_TUNE, VoiceGroup::Global }));
}

std::vector<Parameter*> EditBufferLogicalParts::getModMatrix()
{
  std::vector<Parameter*> ret {};
  for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID({ "MCM", VoiceGroup::Global })->getParameters())
    ret.emplace_back(p);
  return ret;
}

std::vector<Parameter*> EditBufferLogicalParts::getScale()
{
  std::vector<Parameter*> ret {};
  for(auto& p : TestHelper::getEditBuffer()->getParameterGroupByID({ "Scale", VoiceGroup::Global })->getParameters())
    ret.emplace_back(p);
  return ret;
}

size_t EditBufferLogicalParts::createHashOfVector(const std::vector<Parameter*>& v)
{
  size_t ret {};
  for(auto& p : v)
  {
    hash_combine(ret, p->getHash());
  }
  return ret;
}

template <VoiceGroup vg> Parameter* EditBufferLogicalParts::getMonoEnable()
{
  return TestHelper::getEditBuffer()->findParameterByID({ detail::MONO_ENABLE, vg });
}

Parameter* EditBufferLogicalParts::getParameter(const ParameterId& id)
{
  return TestHelper::getEditBuffer()->findParameterByID(id);
}

template <VoiceGroup vg> std::vector<Parameter*> EditBufferLogicalParts::getPartMaster()
{
  std::vector<Parameter*> ret {};
  ret.emplace_back(getPartVolume<vg>());
  ret.emplace_back(getPartTune<vg>());
  return ret;
}
