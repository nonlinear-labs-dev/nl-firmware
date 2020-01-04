#pragma once

#include "BooleanSetting.h"

class PresetDragDropEnabled : public BooleanSetting
{
 private:
  typedef BooleanSetting super;

 public:
  PresetDragDropEnabled(UpdateDocumentContributor &settings);
  virtual ~PresetDragDropEnabled();
};
