#include "Application.h"
#include "groups/HardwareSourcesGroup.h"
#include "PlayModeLayout.h"
#include "RibbonLabel.h"
#include "PlayModeRibbonBehaviourLabel.h"
#include "MCRoutings.h"

PlayModeLayout::PlayModeLayout()
    : super()
{
  addControl(new RibbonLabel(HardwareSourcesGroup::getUpperRibbon1ParameterID(), Rect(25, 1, 78, 14)));
  addControl(new MCRoutings(HardwareSourcesGroup::getUpperRibbon1ParameterID(), Rect(104, 1, 8, 12)));
}

PlayModeLayout::~PlayModeLayout()
{
}
