#pragma once

#include "BooleanSetting.h"

class SendPresetAsPlaycontrollerWriteFallback : public BooleanSetting
{
 private:
  typedef BooleanSetting super;

 public:
  SendPresetAsPlaycontrollerWriteFallback(UpdateDocumentContributor& settings);
  virtual ~SendPresetAsPlaycontrollerWriteFallback();

 private:
  SendPresetAsPlaycontrollerWriteFallback(const SendPresetAsPlaycontrollerWriteFallback& other) = delete;
  SendPresetAsPlaycontrollerWriteFallback& operator=(const SendPresetAsPlaycontrollerWriteFallback&) = delete;
};
