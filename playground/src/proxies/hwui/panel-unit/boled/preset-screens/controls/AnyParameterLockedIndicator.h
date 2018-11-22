#pragma once

#include <tools/DelayedJob.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LockedIndicator.h>
#include "InvertedLabel.h"

class Application;
class Parameter;

class AnyParameterLockedIndicator : public LockedIndicator
{
 public:
  explicit AnyParameterLockedIndicator(const Rect &pos);

  ~AnyParameterLockedIndicator() override = default;

 private:
  void onParameterGroupChanged();
};
