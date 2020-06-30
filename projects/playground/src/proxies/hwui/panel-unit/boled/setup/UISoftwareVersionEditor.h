#pragma once

#include "DisplayEditor.h"

class UISoftwareVersionEditor : public DisplayEditor
{
 public:
  UISoftwareVersionEditor();
  virtual void setPosition(const Rect &) override;
  static const char* getBranch();
  static const char* getHead();
  static const char* getCommitCount();
};
