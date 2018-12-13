#include "ParameterLayout.h"
#include "StaticKnubbelSlider.h"
#include "StaticBarSlider.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModuleCaption.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LockedIndicator.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/ButtonMenu.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <presets/PresetParameter.h>
#include <proxies/hwui/panel-unit/boled/undo/UndoIndicator.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterKnubbelSlider.h>

ParameterLayout2::ParameterLayout2()
    : super(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
  addControl(new ModuleCaption(Rect(0, 0, 64, 13)));
  addControl(new ParameterNameLabel(Rect(BIG_SLIDER_X, 8, 107, 11)));
  addControl(new LockedIndicator(Rect(66, 1, 10, 11)));
  addControl(new UndoIndicator(Rect(4, 15, 10, 5)));
}

Parameter *ParameterLayout2::getCurrentParameter() const
{
  return Application::get().getPresetManager()->getEditBuffer()->getSelected();
}

Parameter *ParameterLayout2::getCurrentEditParameter() const
{
  return getCurrentParameter();
}

bool ParameterLayout2::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if(i == BUTTON_SHIFT)
  {
    if(down)
    {
      handlePresetValueRecall();
    }
    return true;
  }

  if(down)
  {
    switch(i)
    {
      case BUTTON_PRESET:
        Application::get().getHWUI()->undoableSetFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Select));
        return true;

      case BUTTON_STORE:
        Application::get().getHWUI()->undoableSetFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Store));
        return true;

      case BUTTON_INFO:
        Application::get().getHWUI()->undoableSetFocusAndMode(UIMode::Info);
        return true;

      case BUTTON_DEFAULT:
        setDefault();
        return true;

      default:
        break;
    }
  }

  return super::onButton(i, down, modifiers);
}

void ParameterLayout2::setDefault()
{
  if(auto p = getCurrentEditParameter())
    p->setDefaultFromHwui();
}

bool ParameterLayout2::onRotary(int inc, ButtonModifiers modifiers)
{
  if(auto p = getCurrentEditParameter())
  {
    auto scope = p->getUndoScope().startContinuousTransaction(p, "Set '%0'", p->getGroupAndParameterName());
    p->stepCPFromHwui(scope->getTransaction(), inc, modifiers);
    return true;
  }

  return super::onRotary(inc, modifiers);
}

void ParameterLayout2::handlePresetValueRecall()
{
  if(getCurrentEditParameter()->isChangedFromLoaded())
  {
    getOLEDProxy().setOverlay(new ParameterRecallLayout2());
  }
}

ParameterSelectLayout2::ParameterSelectLayout2()
    : super()
{
}

void ParameterSelectLayout2::init()
{
  super::init();
  if(auto c = createCarousel(Rect(195, 0, 58, 64)))
  {
    setCarousel(c);
    m_carousel->setHighlight(true);
  }
}

Carousel *ParameterSelectLayout2::createCarousel(const Rect &rect)
{
  return new ParameterCarousel(rect);
}

void ParameterSelectLayout2::setCarousel(Carousel *c)
{
  if(m_carousel)
    remove(m_carousel);

  m_carousel = addControl(c);
}

Carousel *ParameterSelectLayout2::getCarousel()
{
  return m_carousel;
}

bool ParameterSelectLayout2::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    switch(i)
    {
      case BUTTON_D:
        if(m_carousel)
        {
          if(modifiers[SHIFT] == 1)
          {
            m_carousel->antiTurn();
          }
          else
          {
            m_carousel->turn();
          }
        }

        return true;

      case BUTTON_EDIT:
        Application::get().getHWUI()->undoableSetFocusAndMode(UIMode::Edit);
        return true;
    }
  }

  return super::onButton(i, down, modifiers);
}

ParameterEditLayout2::ParameterEditLayout2()
    : super()
{
  addControl(new InvertedLabel("Edit", Rect(8, 26, 48, 12)))->setHighlight(true);
}

ParameterEditLayout2::~ParameterEditLayout2()
{
}

void ParameterEditLayout2::init()
{
  super::init();

  if((m_menu = createMenu(Rect(195, 1, 58, 62))))
    addControl(m_menu);
}

ButtonMenu *ParameterEditLayout2::getMenu()
{
  return m_menu;
}

bool ParameterEditLayout2::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    if(m_menu)
    {
      if(BUTTON_D == i)
      {
        if(modifiers[SHIFT] == 1)
          m_menu->antiToggle();
        else
          m_menu->toggle();
        return true;
      }
      if(BUTTON_ENTER == i)
      {
        m_menu->doAction();
        return true;
      }
    }

    if(BUTTON_EDIT == i)
    {
      Application::get().getHWUI()->undoableSetFocusAndMode(UIMode::Select);
      return true;
    }
  }

  return super::onButton(i, down, modifiers);
}

ParameterRecallLayout2::ParameterRecallLayout2()
    : super()
{
  m_buttonA = addControl(new Button("", BUTTON_A));
  m_buttonB = addControl(new Button("", BUTTON_B));
  m_buttonC = addControl(new Button("", BUTTON_C));
  m_buttonD = addControl(new Button("", BUTTON_D));

  if(auto p = getCurrentParameter())
  {
    if(auto originalParam = p->getOriginalParameter())
    {
      if(p->getVisualizationStyle() == Parameter::VisualizationStyle::Dot)
        m_slider = addControl(new StaticKnubbelSlider(originalParam->getValue(), p->isBiPolar(),
                                                      Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
      else
        m_slider = addControl(new StaticBarSlider(originalParam->getValue(), p->isBiPolar(),
                                                  Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));

      m_leftValue = addControl(new Label(p->getDisplayString(), Rect(67, 35, 58, 11)));

      auto sc = p->getValue().getScaleConverter();
      auto displayValue = sc->controlPositionToDisplay(originalParam->getValue());
      auto displayString = sc->getDimension().stringize(displayValue);

      m_rightValue = addControl(new Label(displayString, Rect(131, 35, 58, 11)));
    }
  }

  m_recallValue = getCurrentParameter()->getControlPositionValue();

  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &ParameterRecallLayout2::onParameterSelectionChanged));

  updateUI(false);
}

void ParameterRecallLayout2::init()
{
}

bool ParameterRecallLayout2::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if(down && m_paramLikeInPreset && i == BUTTON_C)
    undoRecall();
  else if(down && !m_paramLikeInPreset && i == BUTTON_B)
    doRecall();

  if(i == BUTTON_C || i == BUTTON_B)
    return true;

  if(i == BUTTON_INC || i == BUTTON_DEC)
  {
    auto ret = super::onButton(i, down, modifiers);
    getOLEDProxy().resetOverlay();
    return ret;
  }

  getOLEDProxy().resetOverlay();
  return false;
}

bool ParameterRecallLayout2::onRotary(int inc, ButtonModifiers modifiers)
{
  auto ret = super::onRotary(inc, modifiers);
  getOLEDProxy().resetOverlay();
  return ret;
}

void ParameterRecallLayout2::doRecall()
{
  auto &scope = Application::get().getPresetManager()->getUndoScope();
  auto transactionScope = scope.startTransaction("Recall %0 value from Preset", getCurrentParameter()->getLongName());
  auto transaction = transactionScope->getTransaction();
  if(auto curr = getCurrentParameter())
  {
    if(auto original = curr->getOriginalParameter())
    {
      m_recallString = curr->getDisplayString();
      m_recallValue = curr->getControlPositionValue();
      curr->setCPFromHwui(transaction, original->getValue());
      updateUI(true);
    }
  }
}

void ParameterRecallLayout2::undoRecall()
{
  auto &scope = Application::get().getPresetManager()->getUndoScope();
  auto transactionScope
      = scope.startTransaction("Recall %0 value from Editbuffer", getCurrentParameter()->getLongName());
  auto transaction = transactionScope->getTransaction();
  if(auto curr = getCurrentParameter())
  {
    curr->setCPFromHwui(transaction, m_recallValue);
    updateUI(false);
  }
}

ButtonMenu *ParameterRecallLayout2::createMenu(const Rect &rect)
{
  return nullptr;
}

void ParameterRecallLayout2::updateUI(bool paramLikeInPreset)
{
  m_paramLikeInPreset = paramLikeInPreset;

  if(auto p = getCurrentParameter())
  {
    if(auto originalParam = p->getOriginalParameter())
    {
      if(paramLikeInPreset)
      {
        m_leftValue->setText(p->getDisplayString());
        m_rightValue->setText(m_recallString);
        m_slider->setValue(p->getControlPositionValue(), p->isBiPolar());
        m_rightValue->setHighlight(false);
        m_leftValue->setHighlight(true);
        m_buttonB->setText("");
        m_buttonC->setText("Recall");
      }
      else
      {
        auto sc = p->getValue().getScaleConverter();
        auto displayValue = sc->controlPositionToDisplay(originalParam->getValue());
        auto displayString = sc->getDimension().stringize(displayValue);
        m_leftValue->setText(displayString);
        m_rightValue->setText(p->getDisplayString());
        m_slider->setValue(m_recallValue, p->isBiPolar());
        m_leftValue->setHighlight(false);
        m_rightValue->setHighlight(true);
        m_buttonC->setText("");
        m_buttonB->setText("Recall");
      }
    }
  }
}

void ParameterRecallLayout2::onParameterSelectionChanged(Parameter *oldParam, Parameter *newParam)
{
  m_paramConnection.disconnect();

  if(newParam)
    m_paramConnection = newParam->onParameterChanged(sigc::mem_fun(this, &ParameterRecallLayout2::onParameterChanged));
}

void ParameterRecallLayout2::onParameterChanged(const Parameter *)
{
  updateUI(!getCurrentEditParameter()->isChangedFromLoaded());
}
