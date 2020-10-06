#pragma once
#include "Setting.h"

class SplitPointSyncParameters : public Setting
{
 public:
  SplitPointSyncParameters(Settings& p);
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

  bool get() const;
  void setState(bool s);

 private:
  bool m_state = true;
};
