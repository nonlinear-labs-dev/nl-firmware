#pragma once

#include "ParameterLayout.h"

ENUM(ModRouterLayoutMode, uint8_t, HWAmt, HWSel, HWPos, MC);

class ModulationRouterParameterSelectLayout2 : public virtual ParameterSelectLayout2
{
 public:
  typedef ParameterSelectLayout2 super;
  Carousel *createCarousel(const Rect &rect) override;
  ModulationRouterParameterSelectLayout2();

  ModRouterLayoutMode getMode() const;

 protected:
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
  ModuleCaption *createModuleCaption() const override;
  void init() override;
  bool onRotary(int inc, ButtonModifiers modifiers) override;
  Control *createParameterNameLabel() const override;
  Button *findButtonWithText(const Glib::ustring &name);

 private:
  ModRouterLayoutMode m_mode = ModRouterLayoutMode::HWAmt;
};

class ModulationRouterParameterEditLayout2 : public virtual ParameterEditLayout2
{
 public:
  typedef ParameterEditLayout2 super;
  ModulationRouterParameterEditLayout2();

  ButtonMenu *createMenu(const Rect &rect) override;
};
