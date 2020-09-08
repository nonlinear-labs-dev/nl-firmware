#pragma once
#include "Setting.h"

class TotalRAM : public Setting
{
 public:
  explicit TotalRAM(Settings& s);
  bool persistent() const override;
  void load(const Glib::ustring& text) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

 private:
  Glib::ustring m_display;
};
