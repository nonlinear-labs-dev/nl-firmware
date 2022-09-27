#include <Application.h>
#include <groups/HardwareSourcesGroup.h>
#include <proxies/hwui/base-unit/BaseUnit.h>
#include <proxies/hwui/base-unit/PlayPanel.h>
#include <proxies/hwui/base-unit/soled/MCRoutings.h>
#include <proxies/hwui/base-unit/soled/NoteShiftDisplay.h>
#include <proxies/hwui/base-unit/soled/PlayModeRibbonBehaviourLabel.h>
#include <proxies/hwui/base-unit/soled/PlayOrEditModeLayout.h>
#include <proxies/hwui/base-unit/soled/RibbonLabel.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/DottedLine.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/controls/VerticalLine.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include <proxies/hwui/controls/Label.h>
#include "SelectedRibbonsIndication.h"

PlayOrEditModeLayout::PlayOrEditModeLayout()
    : super(Application::get().getHWUI()->getBaseUnit().getPlayPanel().getSOLED())
{
  addControl(new Label(StringAndSuffix { "Shift" }, Rect(1, 1, 23, 14)));
  addControl(new NoteShiftDisplay(Rect(1, 17, 23, 14)));
  addControl(new VerticalLine(Rect(24, 0, 1, 32)));
  addControl(new DottedLine(Rect(25, 16, 86, 1)));

  addControl(new RibbonLabel(HardwareSourcesGroup::getLowerRibbonParameterID(), Rect(25, 17, 74, 14)));
  addControl(new MCRoutings(HardwareSourcesGroup::getLowerRibbonParameterID(), Rect(99, 20, 8, 8)));

  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      mem_fun(this, &PlayOrEditModeLayout::onParameterSelectionChanged), {});

  addControl(new SelectedRibbonsIndication(Rect(114, 10, 15, 20)));
}

PlayOrEditModeLayout::~PlayOrEditModeLayout() = default;

int PlayOrEditModeLayout::getBehaviourLeft() const
{
  return 109;
}

void PlayOrEditModeLayout::onParameterSelectionChanged(const Parameter* old, Parameter* newP)
{
}
