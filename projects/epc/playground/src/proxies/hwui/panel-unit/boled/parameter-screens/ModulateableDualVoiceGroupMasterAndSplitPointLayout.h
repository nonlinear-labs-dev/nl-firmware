#pragma once

#include <proxies/hwui/buttons.h>
#include "UnmodulatebaleParameterLayouts.h"
#include "ModulateableParameterLayouts.h"

class ModulateableDualVoiceGroupMasterAndSplitPointLayout : public ModulateableParameterSelectLayout2
{
 public:
  using ModulateableParameterSelectLayout2::ModulateableParameterSelectLayout2;

 protected:
  Carousel* createCarousel(const Rect& rect) override;
  ModuleCaption* createModuleCaption() const override;
};

class UnmodulateableDualVoiceGroupMasterAndSplitPointLayout : public UnmodulateableParameterSelectLayout2
{
 public:
  using UnmodulateableParameterSelectLayout2::UnmodulateableParameterSelectLayout2;

 protected:
  Carousel* createCarousel(const Rect& rect) override;
  ModuleCaption* createModuleCaption() const override;
};

class SplitPointParameterLayout : public ModulateableDualVoiceGroupMasterAndSplitPointLayout
{
 protected:
  Control* createParameterValueControl() override;
  void fixValueControl();
 public:
  SplitPointParameterLayout();
};

class SplitPointParameterEditLayout : public ModulateableParameterEditLayout2
{
 protected:
 public:
  SplitPointParameterEditLayout();
  ButtonMenu* createMenu(const Rect& rect) override;

 protected:
  Control* createParameterValueControl() override;
  ModuleCaption* createModuleCaption() const override;
  void fixValueControl();
};