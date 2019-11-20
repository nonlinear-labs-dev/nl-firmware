#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenameVoiceGroupLayout.h>

#include "VoiceGroupLabelItem.h"

inline EditBuffer* getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

VoiceGroup currentVoiceGroup()
{
  return Application::get().getHWUI()->getCurrentVoiceGroup();
}

Glib::ustring getVoiceGroupLabel(VoiceGroup vg)
{
  auto eb = getEditBuffer();
  return eb->getVoiceGroupName(vg);
}

VoiceGroupLabelItem::VoiceGroupLabelItem(const Rect& r)
    : BasicItemWithValueText("Voice Group Label", getVoiceGroupLabel(currentVoiceGroup()), r)
{
}

void VoiceGroupLabelItem::doAction()
{
  auto hwui = Application::get().getHWUI();
  hwui->getPanelUnit().getEditPanel().getBoled().reset(new RenameVoiceGroupLayout(currentVoiceGroup()));
}
