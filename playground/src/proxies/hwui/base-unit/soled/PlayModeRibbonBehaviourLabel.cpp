#include "PlayModeRibbonBehaviourLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include <parameters/RibbonParameter.h>

PlayModeRibbonBehaviourLabel::PlayModeRibbonBehaviourLabel(int paramID, const Rect &rect)
    : super(rect)
{
  getEditBuffer()
      ->findParameterByID(paramID, VoiceGroup::Global)
      ->onParameterChanged(mem_fun(this, &PlayModeRibbonBehaviourLabel::updateText));
}

PlayModeRibbonBehaviourLabel::~PlayModeRibbonBehaviourLabel()
{
}

void PlayModeRibbonBehaviourLabel::updateText(const Parameter *s)
{
  if(auto ribbonParameter = dynamic_cast<const RibbonParameter *>(s))
  {
    switch(ribbonParameter->getRibbonTouchBehaviour())
    {
      case RibbonTouchBehaviour::ABSOLUTE:
        setText("a");
        break;

      case RibbonTouchBehaviour::RELATIVE:
        setText("r");
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
