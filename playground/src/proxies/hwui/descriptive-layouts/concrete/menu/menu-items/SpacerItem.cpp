#include "SpacerItem.h"

SpacerItem::SpacerItem(const Rect &rect)
    : MenuItem(rect)
{
}

void SpacerItem::doAction()
{
}

bool SpacerItem::canEnter()
{
  return false;
}
