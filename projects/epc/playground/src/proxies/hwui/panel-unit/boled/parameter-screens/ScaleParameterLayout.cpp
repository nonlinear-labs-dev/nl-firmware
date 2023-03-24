#include <Application.h>
#include <groups/ParameterGroup.h>
#include <groups/ScaleGroup.h>
#include <http/UndoScope.h>
#include <parameters/Parameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ScaleParameterLayout.h>
#include "use-cases/EditBufferUseCases.h"
#include "parameter_declarations.h"

void ScaleParameterSelectLayout::init()
{
  super::init();
  m_masterButton = addControl(new Button("back..", Buttons::BUTTON_A));
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
      {
        auto eb = Application::get().getPresetManager()->getEditBuffer();
        EditBufferUseCases ebUseCases(*eb);
        ebUseCases.selectParameter({ C15::PID::Master_Volume, VoiceGroup::Global }, true);
      }
        return true;
    }
  }
  return false;
}

Carousel* ScaleParameterSelectLayout::createCarousel(const Rect& rect)
{
  return ParameterSelectLayout2::createCarousel(rect);
}

void ScaleParameterSelectLayout::selectParameter(int inc)
{
  static const auto ids
      = std::vector<int> { C15::PID::Scale_Base_Key, C15::PID::Scale_Offset_0, C15::PID::Scale_Offset_1,
                           C15::PID::Scale_Offset_2, C15::PID::Scale_Offset_3, C15::PID::Scale_Offset_4,
                           C15::PID::Scale_Offset_5, C15::PID::Scale_Offset_6, C15::PID::Scale_Offset_7,
                           C15::PID::Scale_Offset_8, C15::PID::Scale_Offset_9, C15::PID::Scale_Offset_10,
                           C15::PID::Scale_Offset_11 };
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases { *eb };
  auto id = eb->getSelectedParameterNumber();

  auto idIt = std::find(ids.begin(), ids.end(), id);
  if(idIt == ids.end() || (*idIt == C15::PID::Scale_Offset_11 && inc > 0))
  {
    id = C15::PID::Scale_Base_Key;
  }
  else
  {
    auto toEnd = std::distance(idIt, ids.end());
    auto toBegin = std::distance(ids.begin(), idIt);

    if(inc > 0 && toEnd != 1)
      inc = std::min<long>(toEnd, inc);
    else if(inc > 0)
      idIt = ids.begin();

    if(inc < 0 && toBegin != 0)
      inc = std::min<long>(toBegin, inc);
    else if(inc < 0)
      idIt = ids.end();

    idIt += inc;
    id = *idIt;
  }

  ebUseCases.selectParameter({ id, VoiceGroup::Global }, true);
}
