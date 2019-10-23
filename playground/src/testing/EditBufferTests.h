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
  void compareModulateable(const ModulateableParameter *parameter,
                           const nltools::msg::ParameterGroups::ModulateableParameter &mod) const;
  void compareAfterTouch(const AftertouchParameter *p,
                         const nltools::msg::ParameterGroups::AftertouchParameter &a) const;
  void comparePedal(const PedalParameter *pedal, const nltools::msg::ParameterGroups::PedalParameter &p) const;
  void compareRibbon(const RibbonParameter *ribbon, const nltools::msg::ParameterGroups::RibbonParameter &r) const;
  void compareBender(const PitchbendParameter *bender, const nltools::msg::ParameterGroups::BenderParameter &be) const;

  void compareSingleSound(EditBuffer *eb, const nltools::msg::SinglePresetMessage &s) const;

  EditBuffer *m_editBuffer;
};
