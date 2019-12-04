#pragma once

#include "HostAPDSetting.h"

class Settings;

class Passphrase : public HostAPDSetting
{
 private:
  typedef HostAPDSetting super;

 public:
  Glib::ustring getDisplayString() const override;

 public:
  Passphrase(Settings &parent);
  virtual ~Passphrase();

  void dice();
};
