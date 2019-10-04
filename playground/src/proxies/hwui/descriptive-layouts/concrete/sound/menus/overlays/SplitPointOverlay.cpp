#include <parameters/SplitPointParameter.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/controls/SplitPointLabel.h>
#include "SplitPointOverlay.h"
#include "Application.h"
#include "presets/EditBuffer.h"
#include "presets/PresetManager.h"

inline EditBuffer* getEditbuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

inline SplitPointParameter* getSplitPoint()
{
  return dynamic_cast<SplitPointParameter*>(getEditbuffer()->getSplitSoundParameterGroup()->findParameterByID(18700));
}

void SplitPointOverlay::onLeft(bool down)
{
  if(down)
    if(auto splitPoint = getSplitPoint())
    {
      splitPoint->getValue().dec(Initiator::EXPLICIT_HWUI, {});
    }
}

void SplitPointOverlay::onRight(bool down)
{
  if(down)
    if(auto splitPoint = getSplitPoint())
    {
      splitPoint->getValue().inc(Initiator::EXPLICIT_HWUI, {});
    }
}

void SplitPointOverlay::onCommit(bool down)
{
  m_label->setHighlight(down);
}

SplitPointOverlay::SplitPointOverlay(const Rect& r)
    : ArrowIncrementDecrementOverlay(r)
{
  auto labelWidth = r.getWidth() - 20;
  m_label = addControl(new SplitPointLabel({ 11, -1, labelWidth, 13 }));
}
