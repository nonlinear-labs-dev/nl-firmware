#include "EditModeLayout.h"
#include "proxies/hwui/controls/Label.h"
#include "Application.h"
#include "NoteShiftDisplay.h"
#include "RibbonLabel.h"
#include "groups/HardwareSourcesGroup.h"
#include "SelectedParamValueWithFrame.h"
#include "ParamEditModeRibbonBehaviourLabel.h"

EditModeLayout::EditModeLayout()
    : super()
{
  addControl(new SelectedParamValueWithFrame(Rect(29, 2, 70, 13)));
}

EditModeLayout::~EditModeLayout()
{
}
