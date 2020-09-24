#pragma once
#include <proxies/hwui/controls/LabelRegular8.h>

class SplitPointSyncIndicator : public LabelRegular8
{
 public:
  SplitPointSyncIndicator(const Rect& r);
 private:
  void onChange(const Setting*);
};
