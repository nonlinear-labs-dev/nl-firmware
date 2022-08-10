#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>
#include "ModulateableParameterLayouts.h"

class Button;

class BaseScaleParameterSelectLayout : public UnmodulateableParameterSelectLayout2
{
  typedef UnmodulateableParameterSelectLayout2 super;

 public:
  using super::super;

  void init() override;

 private:
  void addButtons() override;
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
  void reset();
  bool resetEnabled() const;
  void selectParameter(int inc);
  void updateMasterButton();
  Button* m_resetButton;
};

class BaseScaleParameterEditLayout : public UnmodulateableParameterEditLayout2
{
  typedef UnmodulateableParameterEditLayout2 super;

 public:
  using super::super;
};

class ScaleParameterSelectLayout : public ModulateableParameterSelectLayout2
{
  typedef ModulateableParameterSelectLayout2 super;

 public:
  using super::super;

  void init() override;

 private:
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
  void selectParameter(int inc);

 protected:
  Carousel* createCarousel(const Rect& rect) override;

 private:
  Button* m_masterButton;
};

class ScaleParameterEditLayout : public UnmodulateableParameterEditLayout2
{
  typedef UnmodulateableParameterEditLayout2 super;

 public:
  using super::super;
};