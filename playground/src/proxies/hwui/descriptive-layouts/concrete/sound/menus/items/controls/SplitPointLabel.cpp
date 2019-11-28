#include <parameters/SplitPointParameter.h>
#include "SplitPointLabel.h"
#include "presets/EditBuffer.h"
#include "presets/PresetManager.h"
#include "proxies/hwui/HWUI.h"

SplitPointLabel::SplitPointLabel(const Rect& r)
    : LabelRegular8(r)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto splitPoint = dynamic_cast<SplitPointParameter*>(eb->findParameterByID({356, VoiceGroup::I})))
    m_connection = splitPoint->onParameterChanged(sigc::mem_fun(this, &SplitPointLabel::onParameterChanged));
}

void SplitPointLabel::onParameterChanged(const Parameter* parameter)
{
  if(auto sp = dynamic_cast<const SplitPointParameter*>(parameter))
  {
    auto displayVG = Application::get().getHWUI()->getCurrentVoiceGroup();
    setText({ sp->getDisplayValue(displayVG), 0 });
  }
}
