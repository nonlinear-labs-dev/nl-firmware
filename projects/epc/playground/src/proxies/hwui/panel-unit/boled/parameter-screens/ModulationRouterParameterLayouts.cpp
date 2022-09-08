#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterBarSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulationRouterParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterEditButtonMenu.h>
#include "use-cases/EditBufferUseCases.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/MacroControlSliderForCurrentModulationRouter.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/FixedTextModuleCaption.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/HWSourceAmountCarousel.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedMacroControlsHWSourceAmount.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedMacroControlsHWSourceValue.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedMacroControlsHWSourceName.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedMacroControlsHWSourceSlider.h"
#include "proxies/hwui/controls/DottedLine.h"
#include "groups/MacroControlMappingGroup.h"

ModulationRouterParameterSelectLayout2::ModulationRouterParameterSelectLayout2()
    : super()
{
  if(auto modRouter = dynamic_cast<ModulationRoutingParameter *>(getCurrentParameter()))
  {
    nltools::Log::error("selecting modRouter:", modRouter->getSourceParameter()->getLongName(), "to",
                        modRouter->getTargetParameter()->getLongName());
    modRouter->getTargetParameter()->setUiSelectedHardwareSource(modRouter->getSourceParameter()->getID());
    nltools::Log::error("uiSelected:", modRouter->getTargetParameter()->getUiSelectedHardwareSource());
  }

  addControl(new Button("HW Pos", Buttons::BUTTON_A));
  addControl(new Button("HW Sel", Buttons::BUTTON_B));
  addControl(new Button("HW Amt", Buttons::BUTTON_C));
  highlightButtonWithCaption("HW Amt");

  addControl(new MacroControlSliderForCurrentModulationRouter(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
  addControl(new SelectedMacroControlsHWSourceSlider(Rect(8, 25, 48, 4)));
  addControl(new SelectedMacroControlsHWSourceValue(Rect(0, BUTTON_VALUE_Y_POSITION, 64, 12)));
  addControl(new SelectedMacroControlsHWSourceName(Rect(64, BUTTON_VALUE_Y_POSITION, 64, 12)));
  addControl(new SelectedMacroControlsHWSourceAmount(Rect(131, BUTTON_VALUE_Y_POSITION, 58, 12)))->setHighlight(true);
  //addControl(new DottedLine(Rect(60, 27, 13, 1)));

  highlight<ParameterNameLabel>();
  highlight<SelectedParameterBarSlider>();
  highlight<SelectedParameterValue>();
}

Control *ModulationRouterParameterSelectLayout2::createParameterNameLabel() const
{
  return new ParameterNameLabelForMCOfModulationRouter(Rect(BIG_SLIDER_X - 2, 8, BIG_SLIDER_WIDTH + 4, 11));
}

void ModulationRouterParameterSelectLayout2::init()
{
  ParameterSelectLayout2::init();
  getCarousel()->setHighlight(false);
}

ModuleCaption *ModulationRouterParameterSelectLayout2::createModuleCaption() const
{
  return new FixedTextModuleCaption(Rect(0, 0, 64, 13), "MC HW Amt");
}

Carousel *ModulationRouterParameterSelectLayout2::createCarousel(const Rect &rect)
{
  return new HWSourceAmountCarousel(Rect(195, 0, 58, 64));
}

bool ModulationRouterParameterSelectLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases { *eb };

  if(down)
  {
    if(i == Buttons::BUTTON_A)
    {
      if(auto p = dynamic_cast<ModulationRoutingParameter *>(getCurrentParameter()))
      {
        ebUseCases.selectParameter(p->getSourceParameter());
      }

      return true;
    }

    if(i == Buttons::BUTTON_B)
    {
      return true;
    }

    if(i == Buttons::BUTTON_C)
    {
      if(auto hwAmtButton = findButtonWithText("HW Amt"))
      {
        m_mode = hwAmtButton->isHighlight() ? ModRouterLayoutMode::MC : ModRouterLayoutMode::HWAmt;
        if(auto p = dynamic_cast<ModulationRoutingParameter *>(getCurrentParameter()))
        {
          auto mc = p->getTargetParameter();
          ebUseCases.selectParameter(mc);
        }
      }
      return true;
    }

    if(i == Buttons::BUTTON_D)
    {
      m_mode = ModRouterLayoutMode::HWAmt;
      if(auto p = dynamic_cast<ModulationRoutingParameter *>(getCurrentParameter()))
      {
        p->getTargetParameter()->toggleUiSelectedHardwareSource(modifiers[ButtonModifier::SHIFT] ? -1 : 1);
        auto currentMC = p->getTargetParameter();
        auto newHWSrc = p->getTargetParameter()->getUiSelectedHardwareSource();
        if(auto g = dynamic_cast<MacroControlMappingGroup*>(p->getParent()))
        {
          auto hwSrc = eb->findAndCastParameterByID<PhysicalControlParameter>(newHWSrc);
          auto modP = g->getModulationRoutingParameterFor(hwSrc, currentMC);
          ebUseCases.selectParameter(modP);
        }
      }
      return true;
    }
  }

  return super::onButton(i, down, modifiers);
}

bool ModulationRouterParameterSelectLayout2::onRotary(int inc, ButtonModifiers modifiers)
{
  return ParameterLayout2::onRotary(inc, modifiers);
}

ModRouterLayoutMode ModulationRouterParameterSelectLayout2::getMode() const
{
  return m_mode;
}

Button* ModulationRouterParameterSelectLayout2::findButtonWithText(const Glib::ustring &name)
{
  for(auto b : getControls<Button>())
  {
    if(b->getText().text == name)
      return b.get();
  }
  return nullptr;
}

ModulationRouterParameterEditLayout2::ModulationRouterParameterEditLayout2()
    : super()
{
  addControl(new Button("HW Pos", Buttons::BUTTON_A));
  addControl(new Button("HW Sel", Buttons::BUTTON_B));
  addControl(new Button("HW Amt", Buttons::BUTTON_C));

  addControl(new SelectedParameterBarSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
  addControl(createParameterValueControl());

  highlight<ParameterNameLabel>();
  highlight<SelectedParameterBarSlider>();
  highlight<SelectedParameterValue>();
}

ButtonMenu *ModulationRouterParameterEditLayout2::createMenu(const Rect &rect)
{
  return new ParameterEditButtonMenu(rect);
}
