#pragma once

#include "EnumSetting.h"
#include <nltools/Types.h>

class ParameterEditModeRibbonBehaviour : public EnumSetting<ParameterEditModeRibbonBehaviours>
{
 private:
  typedef EnumSetting<ParameterEditModeRibbonBehaviours> super;

 public:
  explicit ParameterEditModeRibbonBehaviour(Settings &settings);
  ~ParameterEditModeRibbonBehaviour() override;

  bool set(ParameterEditModeRibbonBehaviours m) override;
  void sendRelatedSettingToPlaycontroller() const;

  void syncExternals(SendReason reason) const override;

  ParameterEditModeRibbonBehaviour(const ParameterEditModeRibbonBehaviour &other) = delete;
  ParameterEditModeRibbonBehaviour &operator=(const ParameterEditModeRibbonBehaviour &) = delete;

 private:
  const std::vector<Glib::ustring> &enumToString() const override;
  const std::vector<Glib::ustring> &enumToDisplayString() const override;
};
