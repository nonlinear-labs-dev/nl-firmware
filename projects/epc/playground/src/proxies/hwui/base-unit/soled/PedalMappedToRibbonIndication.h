#pragma once
#include <proxies/hwui/controls/Label.h>
#include <ParameterId.h>

class Parameter;

class PedalMappedToRibbonIndication : public Label
{
 public:
  explicit PedalMappedToRibbonIndication(const Rect& r);
};