#pragma once
#include "BasicItem.h"

class SpacerItem : public MenuItem {
public:
  SpacerItem(const Rect &rect);
  void doAction() override;
  bool canEnter() override;
};
