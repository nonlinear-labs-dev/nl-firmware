#pragma once

#include <device-settings/BooleanSetting.h>

class SignalFlowIndicationSetting : public BooleanSetting
{
 public:
  explicit SignalFlowIndicationSetting(UpdateDocumentContributor& settings);
  virtual ~SignalFlowIndicationSetting() = default;
  SignalFlowIndicationSetting(const SignalFlowIndicationSetting& other) = delete;
  SignalFlowIndicationSetting& operator=(const SignalFlowIndicationSetting&) = delete;
};
