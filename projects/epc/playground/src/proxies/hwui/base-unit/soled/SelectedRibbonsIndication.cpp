#include "SelectedRibbonsIndication.h"
#include "Application.h"
#include "parameters/RibbonParameter.h"
#include <device-settings/SelectedRibbonsSetting.h>
#include <proxies/hwui/FrameBuffer.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>

SelectedRibbonsIndication::SelectedRibbonsIndication(const Rect& pos)
    : Control(pos)
{
  Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->onChange(
      sigc::mem_fun(this, &SelectedRibbonsIndication::onSettingChanged));

  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  eb->getParameterGroupByID({ "MCM", VoiceGroup::Global })
      ->onGroupChanged(sigc::mem_fun(this, &SelectedRibbonsIndication::setDirty));
}

void SelectedRibbonsIndication::onSettingChanged(const Setting* s)
{
  if(auto setting = dynamic_cast<const SelectedRibbonsSetting*>(s))
  {
    m_isRibbon1_2_selected = setting->get() == SelectedRibbons::Ribbon1_2;
  }
}

void drawDottedOrFull(FrameBuffer& fb, int l, int t, int length, bool state)
{
  if(state)
    fb.drawHorizontalLine(l, t, length);
  else
    fb.drawDottedLine(l, t, length);
}

bool isRibbonAssigned(int pid)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto ribbon = eb->findAndCastParameterByID<RibbonParameter>({ pid, VoiceGroup::Global });
  return ribbon->getMCAssignedToThisHW() != nullptr;
}

bool SelectedRibbonsIndication::redraw(FrameBuffer& fb)
{
  fb.setColor(FrameBufferColors::C43);
  fb.fillRect(getPosition());

  fb.setColor(FrameBufferColors::C255);

  auto l = getPosition().getLeft();
  auto top = getPosition().getTop();

  drawDottedOrFull(fb, l, top, 9, isRibbonAssigned(C15::PID::Ribbon_1));
  drawDottedOrFull(fb, l, top + 4, 9, isRibbonAssigned(C15::PID::Ribbon_2));
  drawDottedOrFull(fb, l, top + 8, 9, isRibbonAssigned(C15::PID::Ribbon_3));
  drawDottedOrFull(fb, l, top + 12, 9, isRibbonAssigned(C15::PID::Ribbon_4));

  if(m_isRibbon1_2_selected)
    fb.drawVerticalLine(l + 11, top + 0, 5);
  else
    fb.drawVerticalLine(l + 11, top + 8, 5);

  return Control::redraw(fb);
}
