#include <Application.h>
#include <groups/ParameterGroup.h>
#include <http/UndoScope.h>
#include <parameters/Parameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ScaleParameterLayout.h>

void toggleHightlight(Control* c);

void ScaleParameterSelectLayout::init ()
{
  super::init ();
}

void ScaleParameterSelectLayout::addButtons ()
{
  m_resetButton = addControl (new Button ("Reset", BUTTON_A));
  addControl (new Button ("<", BUTTON_B));
  addControl (new Button (">", BUTTON_C));
}

bool ScaleParameterSelectLayout::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if (super::onButton (i, down, modifiers))
    return true;

  if (down)
  {
    switch (i)
    {
      case BUTTON_A:
        toggleHightlight(m_resetButton);
        return true;

      case BUTTON_B:
        selectParameter (-1);
        return true;

      case BUTTON_C:
        selectParameter (+1);
        return true;

      case BUTTON_ENTER:
        if(m_resetButton->isHighlight()) {
          reset();
          m_resetButton->setHighlight(false);
          return true;
        }
    }
  }
  return false;
}

void ScaleParameterSelectLayout::reset()
{
  auto eb = Application::get ().getPresetManager ()->getEditBuffer ();
  auto scope = Application::get ().getUndoScope ()->startTransaction ("Reset Custom Scale");
  eb->getParameterGroupByID("Scale")->undoableReset(scope->getTransaction(), Initiator::EXPLICIT_HWUI);
}

void ScaleParameterSelectLayout::selectParameter (int inc)
{
  const auto min = 312;
  const auto max = 323;
  const auto range = (max - min) + 1;

  auto eb = Application::get ().getPresetManager ()->getEditBuffer ();
  auto id = eb->getSelected ()->getID () + inc;

  while (id > max)
    id -= range;

  while (id < min)
    id += range;

  eb->undoableSelectParameter (id);
}

void toggleHightlight(Control* c) {
  c->setHighlight(!c->isHighlight());
}

