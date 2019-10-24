#pragma once

#include <nltools/messaging/Message.h>

class EditBuffer;
class Parameter;
class ModulateableParameter;
class AftertouchParameter;
class PedalParameter;
class RibbonParameter;
class PitchbendParameter;

class EditBufferTests
{
 public:
  explicit EditBufferTests(EditBuffer *eb);

 private:
  void compareUnmodulateable(const Parameter *parameter,
                             const nltools::msg::ParameterGroups::Parameter &presetParameter) const;
  void compareUnmodulateable(const nltools::msg::ParameterGroups::Parameter &p1,
                             const nltools::msg::ParameterGroups::Parameter &p2) const;

  void compareModulateable(const ModulateableParameter *parameter,
                           const nltools::msg::ParameterGroups::ModulateableParameter &mod) const;
  void compareModulateable(const nltools::msg::ParameterGroups::ModulateableParameter &p1,
                           const nltools::msg::ParameterGroups::ModulateableParameter &p2) const;

  void compareAfterTouch(const AftertouchParameter *p,
                         const nltools::msg::ParameterGroups::AftertouchParameter &a) const;

  void compareAfterTouch(const nltools::msg::ParameterGroups::AftertouchParameter &p1,
                         const nltools::msg::ParameterGroups::AftertouchParameter &p2) const;

  void comparePedal(const PedalParameter *pedal, const nltools::msg::ParameterGroups::PedalParameter &p) const;
  void comparePedal(const nltools::msg::ParameterGroups::PedalParameter &p1,
                    const nltools::msg::ParameterGroups::PedalParameter &p2) const;

  void compareRibbon(const RibbonParameter *ribbon, const nltools::msg::ParameterGroups::RibbonParameter &r) const;
  void compareRibbon(const nltools::msg::ParameterGroups::RibbonParameter &p1,
                     const nltools::msg::ParameterGroups::RibbonParameter &p2) const;

  void compareBender(const PitchbendParameter *bender, const nltools::msg::ParameterGroups::BenderParameter &be) const;
  void compareBender(const nltools::msg::ParameterGroups::BenderParameter &p1,
                     const nltools::msg::ParameterGroups::BenderParameter &p2) const;

  void compareSingleSound(EditBuffer *eb, const nltools::msg::SinglePresetMessage &s) const;
  void compareSingleSound(const nltools::msg::SinglePresetMessage &s1,
                          const nltools::msg::SinglePresetMessage &s2) const;

  EditBuffer *m_editBuffer;

  void compareSplitSoundToSingleOrigin(const nltools::msg::SinglePresetMessage &original,
                                       const nltools::msg::SplitPresetMessage &converted);

  void compareLayerSoundToOriginalSingle(const nltools::msg::LayerPresetMessage& layer,
                                         const nltools::msg::SinglePresetMessage& single);

  void initEditBuffer();

  void convertSingleToSplit() const;

  template <typename P1, typename P2> void assertScaleGroupsSame(const P1 &p1, const P2 &p2) const
  {
    size_t scale = 0;
    for(auto &_ : p1.scale)
    {
      auto &og = p2.scale[scale];
      auto &conv = p1.scale[scale];
      scale++;

      compareUnmodulateable(og, conv);
    }
  }

  template <typename P1, typename P2> void assertGeneralDualParameterGroups(const P1 &p1, const P2 &p2) const
  {
    for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
    {
      const auto vgIndex = static_cast<int>(vg);

      size_t unmod = 0;
      size_t mod = 0;
      size_t macro = 0;
      size_t pedal = 0;
      size_t bender = 0;
      size_t ribbon = 0;
      size_t after = 0;

      for(auto &_ : p1.unmodulateables[vgIndex])
      {
        auto &og = p2.unmodulateables[unmod];
        auto &conv = p1.unmodulateables[vgIndex][unmod];
        unmod++;

        compareUnmodulateable(og, conv);
      }

      for(auto &_ : p1.modulateables[vgIndex])
      {
        auto &og = p2.modulateables[mod];
        auto &conv = p1.modulateables[vgIndex][mod];
        mod++;

        compareModulateable(og, conv);
      }

      for(auto &_ : p1.macros[vgIndex])
      {
        auto &og = p2.macros[macro];
        auto &conv = p1.macros[vgIndex][macro];
        macro++;

        compareUnmodulateable(og, conv);
      }

      for(auto &_ : p1.ribbons[vgIndex])
      {
        auto &og = p2.ribbons[ribbon];
        auto &conv = p1.ribbons[vgIndex][ribbon];
        ribbon++;

        compareRibbon(og, conv);
      }

      for(auto &_ : p1.bender[vgIndex])
      {
        auto &og = p2.bender[bender];
        auto &conv = p1.bender[vgIndex][bender];
        bender++;

        compareBender(og, conv);
      }

      for(auto &_ : p1.pedals[vgIndex])
      {
        auto &og = p2.pedals[pedal];
        auto &conv = p1.pedals[vgIndex][pedal];
        pedal++;

        comparePedal(og, conv);
      }

      for(auto &_ : p1.aftertouch[vgIndex])
      {
        auto &og = p2.aftertouch[after];
        auto &conv = p1.aftertouch[vgIndex][after];
        after++;

        compareAfterTouch(og, conv);
      }
    }
  }
};
