#pragma once

#include "BooleanSetting.h"

class HighlightChangedParametersSetting : public BooleanSetting
{
 public:
  explicit HighlightChangedParametersSetting(UpdateDocumentContributor& parent);
};

class ForceHighlightChangedParametersSetting : public BooleanSetting
{
 public:
  explicit ForceHighlightChangedParametersSetting(UpdateDocumentContributor& parent);
  bool persistent() const override;
};
