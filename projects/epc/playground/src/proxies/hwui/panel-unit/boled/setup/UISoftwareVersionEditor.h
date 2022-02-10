#pragma once

#include "DisplayEditor.h"

class UISoftwareVersionEditor : public DisplayEditor
{
 public:
  UISoftwareVersionEditor();
  void setPosition(const Rect&) override;
  static const char* getBranch();
  static const char* getHead();
  static const char* getCommitCount();
  static const char* getCommitDate();
  bool onRotary(int inc, ButtonModifiers modifiers) override;

 private:
  std::vector<Glib::ustring> m_ogOrder;
  bool m_showLast = false;

  void sortOrder();
};
