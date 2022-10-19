#include "PlayModeRibbonBehaviourLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "device-settings/SelectedRibbonsSetting.h"
#include <parameters/RibbonParameter.h>

PlayModeRibbonBehaviourLabel::PlayModeRibbonBehaviourLabel(const ParameterId &paramID, const Rect &rect)
    : super(rect)
    , m_ribbonParamID(paramID)
{
  Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->onChange(
      sigc::mem_fun(this, &PlayModeRibbonBehaviourLabel::onRibbonSelectionChanged));
}

PlayModeRibbonBehaviourLabel::~PlayModeRibbonBehaviourLabel()
{
}

void PlayModeRibbonBehaviourLabel::onRibbonSelectionChanged(const Setting *s)
{
  if(auto selected = dynamic_cast<const SelectedRibbonsSetting *>(s))
  {
    auto isOneAndTwo = selected->get() == SelectedRibbons::Ribbon1_2;

    if(isOneAndTwo)
    {
      if(m_ribbonParamID.getNumber() == C15::PID::Ribbon_3)
        m_ribbonParamID = { C15::PID::Ribbon_1, VoiceGroup::Global };
      else if(m_ribbonParamID.getNumber() == C15::PID::Ribbon_4)
        m_ribbonParamID = { C15::PID::Ribbon_2, VoiceGroup::Global };
    }
    else
    {
      if(m_ribbonParamID.getNumber() == C15::PID::Ribbon_1)
        m_ribbonParamID = { C15::PID::Ribbon_3, VoiceGroup::Global };
      else
        m_ribbonParamID = { C15::PID::Ribbon_4, VoiceGroup::Global };
    }
  }

  m_connection = getEditBuffer()
                     ->findParameterByID(m_ribbonParamID)
                     ->onParameterChanged(mem_fun(this, &PlayModeRibbonBehaviourLabel::updateText));
  setDirty();
}

void PlayModeRibbonBehaviourLabel::updateText(const Parameter *s)
{
  if(auto ribbonParameter = dynamic_cast<const RibbonParameter *>(s))
  {
    switch(ribbonParameter->getRibbonTouchBehaviour())
    {
      case RibbonTouchBehaviour::ABSOLUTE:
        setText(StringAndSuffix { "a" });
        break;

      case RibbonTouchBehaviour::RELATIVE:
        setText(StringAndSuffix { "r" });
        break;

      default:
        g_assert_not_reached();
        break;
    }
  }
}

EditBuffer *PlayModeRibbonBehaviourLabel::getEditBuffer() const
{
  return Application::get().getPresetManager()->getEditBuffer();
}
