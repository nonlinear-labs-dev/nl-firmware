#pragma once

#include "EnumSetting.h"
#include <nltools/Types.h>

class BooleanSetting : public EnumSetting<BooleanSettings>
{
 private:
  typedef EnumSetting<BooleanSettings> super;

 public:
  BooleanSetting(UpdateDocumentContributor &settings, bool def);
  virtual ~BooleanSetting();

  virtual void toggle();

  void incDec(int inc, bool wrap);

  bool get() const;
  const std::vector<Glib::ustring> &enumToDisplayString() const override;

  BooleanSetting(const BooleanSetting &other) = delete;
  BooleanSetting &operator=(const BooleanSetting &) = delete;

 private:
  const std::vector<Glib::ustring> &enumToString() const override;
};
