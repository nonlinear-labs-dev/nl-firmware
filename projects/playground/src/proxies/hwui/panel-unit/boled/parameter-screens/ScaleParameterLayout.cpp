#include <Application.h>
#include <groups/ParameterGroup.h>
#include <groups/ScaleGroup.h>
#include <http/UndoScope.h>
#include <parameters/Parameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ScaleParameterLayout.h>

void toggleHightlight(Control* c);

void ScaleParameterSelectLayout::init()
{
  super::init();

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->getParameterGroupByID({ "Scale", VoiceGroup::Global })
      ->onGroupChanged(sigc::mem_fun(this, &ScaleParameterSelectLayout::updateResetButton));
}

void ScaleParameterSelectLayout::addButtons()
{
  m_resetButton = addControl(new Button("", Buttons::BUTTON_A));
  addControl(new Button("<", Buttons::BUTTON_B));
  addControl(new Button(">", Buttons::BUTTON_C));
  updateResetButton();
}

void ScaleParameterSelectLayout::updateResetButton()
{
  m_resetButton->setText(resetEnabled() ? "Reset" : "");
}

bool ScaleParameterSelectLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(super::onButton(i, down, modifiers))
    return true;

  if(down)
  {
    switch(i)
    {

      case Buttons::BUTTON_A:
        if(resetEnabled())
        {
          toggleHightlight(m_resetButton);
        }
        return true;

      case Buttons::BUTTON_B:
        selectParameter(-1);
        return true;

      case Buttons::BUTTON_C:
        selectParameter(+1);
        return true;

      case Buttons::BUTTON_ENTER:
        if(m_resetButton->isHighlight())
        {
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
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto scope = Application::get().getUndoScope()->startTransaction("Reset Custom Scale");
  eb->getParameterGroupByID({ "Scale", VoiceGroup::Global })
      ->undoableReset(scope->getTransaction(), Initiator::EXPLICIT_HWUI);
}

bool ScaleParameterSelectLayout::resetEnabled() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto scaleGroup = dynamic_cast<ScaleGroup*>(eb->getParameterGroupByID({ "Scale", VoiceGroup::Global }));
  return scaleGroup->isAnyOffsetChanged();
}

void ScaleParameterSelectLayout::selectParameter(int inc)
{
  const auto min = 312;
  const auto max = 323;
  const auto range = (max - min) + 1;

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto selectedID = eb->getSelected()->getID();
  auto id = selectedID.getNumber() + inc;

  while(id > max)
    id -= range;

  while(id < min)
    id += range;

  eb->undoableSelectParameter({ id, selectedID.getVoiceGroup() });
}

void toggleHightlight(Control* c)
{
  c->setHighlight(!c->isHighlight());
}