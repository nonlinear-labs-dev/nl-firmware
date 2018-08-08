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
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterKnubbelSlider.h>

Parameter* getOriginalParameter(Parameter* currentParam) {
  auto pm = Application::get().getPresetManager();
  auto uuid = pm->getEditBuffer()->getUUIDOfLastLoadedPreset();
  if(auto originalPreset = pm->findPreset(uuid)) {
    if(auto originalParameter = originalPreset->findParameterByID(currentParam->getID())) {
      return originalParameter;
    }
  }
  return nullptr;
}

ParameterLayout2::ParameterLayout2 () :
    super (Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
  addControl (new ModuleCaption (Rect (0, 0, 64, 13)));
  addControl (new ParameterNameLabel (Rect (72, 8, 112, 11)));
  addControl (new LockedIndicator (Rect (68, 0, 4, 13)));
}

Parameter * ParameterLayout2::getCurrentParameter () const
{
  return Application::get ().getPresetManager ()->getEditBuffer ()->getSelected ();
}

Parameter * ParameterLayout2::getCurrentEditParameter () const
{
  return getCurrentParameter ();
}

bool ParameterLayout2::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if(i == BUTTON_SHIFT) {
    if(down) {
      handlePresetValueRecall();
    }
    return true;
  }

  if (down)
  {
    switch (i)
    {
    case BUTTON_PRESET:
      Application::get ().getHWUI ()->undoableSetFocusAndMode (FocusAndMode (UIFocus::Presets, UIMode::Select));
      return true;

    case BUTTON_STORE:
      Application::get ().getHWUI ()->undoableSetFocusAndMode (FocusAndMode (UIFocus::Presets, UIMode::Store));
      return true;

    case BUTTON_INFO:
      Application::get ().getHWUI ()->undoableSetFocusAndMode (UIMode::Info);
      return true;

    case BUTTON_DEFAULT:
      setDefault ();
      return true;
    }
  }

  return super::onButton (i, down, modifiers);
}

void ParameterLayout2::setDefault ()
{
  if (auto p = getCurrentEditParameter ())
    p->setDefaultFromHwui ();
}

bool ParameterLayout2::onRotary (int inc, ButtonModifiers modifiers)
{
  if (auto p = getCurrentEditParameter ())
  {
    auto scope = p->getUndoScope ().startContinuousTransaction (p, "Set '%0'", p->getGroupAndParameterName ());
    p->stepCPFromHwui (scope->getTransaction (), inc, modifiers);
    return true;
  }

  return super::onRotary (inc, modifiers);
}

void ParameterLayout2::handlePresetValueRecall() {
  if(isCurrentParamDiffFromLoaded()) {
    getOLEDProxy().setOverlay(new ParameterRecallLayout2());
  }
}

bool ParameterLayout2::isCurrentParamDiffFromLoaded() const {
  if(auto currentParam = getCurrentEditParameter()) {
    if(auto originalParameter = getOriginalParameter(currentParam))
      return currentParam->getControlPositionValue() != originalParameter->getControlPositionValue();
  }
  return false;
}

ParameterSelectLayout2::ParameterSelectLayout2 () :
    super ()
{
}

void ParameterSelectLayout2::init ()
{
  super::init ();

  setCarousel(createCarousel (Rect (195, 0, 58, 64)));
  m_carousel->setHighlight (true);
}

Carousel *ParameterSelectLayout2::createCarousel (const Rect &rect)
{
  return new ParameterCarousel (rect);
}

void ParameterSelectLayout2::setCarousel(Carousel *c)
{
  if(m_carousel)
    remove(m_carousel);

  m_carousel = addControl (c);
}

Carousel *ParameterSelectLayout2::getCarousel()
{
  return m_carousel;
}

bool ParameterSelectLayout2::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if (down)
  {
    switch(i)
    {
      case BUTTON_D:
        if(m_carousel)
        {
          if(modifiers[SHIFT] == 1)
          {
            m_carousel->antiTurn ();
          }
          else
          {
            m_carousel->turn ();
          }
        }

        return true;

      case BUTTON_EDIT:
        Application::get ().getHWUI ()->undoableSetFocusAndMode (UIMode::Edit);
        return true;
    }
  }

  return super::onButton (i, down, modifiers);
}

ParameterEditLayout2::ParameterEditLayout2 () :
    super ()
{
  addControl (new InvertedLabel ("Edit", Rect (8, 26, 48, 12)))->setHighlight (true);
}

ParameterEditLayout2::~ParameterEditLayout2 ()
{
}

void ParameterEditLayout2::init ()
{
  super::init ();

  if ((m_menu = createMenu (Rect (195, 1, 58, 62))))
    addControl (m_menu);
}

ButtonMenu *ParameterEditLayout2::getMenu ()
{
  return m_menu;
}

bool ParameterEditLayout2::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if (down)
  {
    if (m_menu)
    {
      if (BUTTON_D == i)
      {
        if(modifiers[SHIFT] == 1)
          m_menu->antiToggle();
        else
          m_menu->toggle ();
        return true;
      }
      if (BUTTON_ENTER == i)
      {
        m_menu->doAction ();
        return true;
      }
    }

    if (BUTTON_EDIT == i)
    {
      Application::get ().getHWUI ()->undoableSetFocusAndMode (UIMode::Select);
      return true;
    }
  }

  return super::onButton (i, down, modifiers);
}

ParameterRecallLayout2::ParameterRecallLayout2() : super() {
  addControl(new Button("Recall", BUTTON_A));
  addControl(new Button("", BUTTON_B));
  addControl(new Button("", BUTTON_C));
  addControl(new Button("", BUTTON_D));

  if (auto p = getCurrentParameter ())
  {
    if(auto originalParam = getOriginalParameter(getCurrentParameter())) {
      if (p->getVisualizationStyle() == Parameter::VisualizationStyle::Dot)
        addControl (new StaticKnubbelSlider(originalParam, Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));

      else
        addControl (new StaticBarSlider (originalParam ,Rect (BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));

      addControl (new Label (getOriginalParameter(getCurrentParameter())->getDisplayString(), Rect (90, 33, 76, 12)));
    }
  }

}

void ParameterRecallLayout2::init() {
  highlight<StaticKnubbelSlider>();
  highlight<StaticBarSlider>();
  highlight<Label>();
  lowlight<Button>();
}

bool ParameterRecallLayout2::onButton (int i, bool down, ButtonModifiers modifiers) {
  auto up = !down;

  if(down && i == BUTTON_A) {
      doRecall();
      return true;
  }
  else if(up && BUTTON_SHIFT == i) {
          getOLEDProxy().resetOverlay();
          return true;
  }
  return false;
}

bool ParameterRecallLayout2::onRotary (int inc, ButtonModifiers modifiers) {
  getOLEDProxy().resetOverlay();
  return true;
}


void ParameterRecallLayout2::doRecall() {
  auto& scope = Application::get().getPresetManager()->getUndoScope();
  auto transactionScope = scope.startTransaction("Recall %0 value from Preset", getCurrentParameter()->getLongName());
  auto transaction = transactionScope->getTransaction();
  if(auto curr = getCurrentParameter()) {
    if(auto original = getOriginalParameter(curr)) {
      curr->setCPFromHwui(transaction, original->getControlPositionValue());
      getOLEDProxy().resetOverlay();
    }
  }
}

ButtonMenu *ParameterRecallLayout2::createMenu (const Rect &rect) {
  return nullptr;
}

