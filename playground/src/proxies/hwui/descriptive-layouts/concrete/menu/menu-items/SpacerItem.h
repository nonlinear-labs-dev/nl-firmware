#pragma once
#include "BasicItem.h"

class SpacerItem : public MenuItem {
public:
  SpacerItem();
  void doAction() override;
  bool canEnter() override;
};
