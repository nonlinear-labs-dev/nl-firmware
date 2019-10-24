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
  void compareSingleSound(const nltools::msg::SinglePresetMessage &s1, const nltools::msg::SinglePresetMessage &s2) const;

  EditBuffer *m_editBuffer;

  void compareSplitSoundToSingleOrigin(const nltools::msg::SinglePresetMessage &original,
                                       const nltools::msg::SplitPresetMessage &converted);

  void compareLayerSoundToOriginalSingle(const nltools::msg::LayerPresetMessage layer,
                                         const nltools::msg::SinglePresetMessage single);
};
