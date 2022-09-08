#pragma once

#include "Setting.h"
#include "DateTimeAdjustment.h"

class Settings;

class DeviceName : public Setting
{
 private:
  typedef Setting super;

 public:
  explicit DeviceName(Settings &parent);
  ~DeviceName() override;

  Glib::ustring getDisplayString() const override;

  void sanitize(DateTimeAdjustment *adj);

  void load(const Glib::ustring &text, Initiator initiator) override;
  Glib::ustring save() const override;

  void set(Glib::ustring name);
  const Glib::ustring &get() const;

 private:
  Settings& m_settings;
  Glib::ustring m_name;
};
