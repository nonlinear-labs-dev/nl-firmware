#include "EditModeLayout.h"
#include "proxies/hwui/controls/Label.h"
#include "Application.h"
#include "NoteShiftDisplay.h"
#include "RibbonLabel.h"
#include "groups/HardwareSourcesGroup.h"
#include "SelectedParamValueWithFrame.h"
#include "ParamEditModeRibbonBehaviourLabel.h"
#include "SelectedRibbonsIndication.h"
#include "proxies/hwui/controls/DottedLine.h"

EditModeLayout::EditModeLayout()
    : super()
{
  addControl(new SelectedParamValueWithFrame(Rect(29, 2, 87, 13)));
  findControlOfType<SelectedRibbonsIndication>()->setVisible(false);
  addControl(new ParamEditModeRibbonBehaviourLabel(Rect(118, 2, 10, 13)));
  addControl(new DottedLine(Rect(111, 16, 17, 1)));
};

EditModeLayout::~EditModeLayout()
{
}
