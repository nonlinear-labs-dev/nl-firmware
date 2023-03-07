#pragma once

#include "EnumSetting.h"
#include <nltools/Types.h>

class BenderCurve : public EnumSetting<BenderCurves>
{
 private:
  typedef EnumSetting<BenderCurves> super;

 public:
  explicit BenderCurve(UpdateDocumentContributor &settings);
  ~BenderCurve() override;

  void syncExternals(SendReason reason) const override;

  const std::vector<Glib::ustring> &enumToDisplayString() const override;

 private:
  const std::vector<Glib::ustring> &enumToString() const override;
};
