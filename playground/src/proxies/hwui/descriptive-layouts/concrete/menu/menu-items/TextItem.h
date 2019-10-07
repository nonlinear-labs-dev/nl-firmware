#pragma once

#include "BasicItem.h"

class TextItem : public BasicItem {
public:
  TextItem(const std::string& text);
  bool canEnter() override;

  void doAction() override;
};
