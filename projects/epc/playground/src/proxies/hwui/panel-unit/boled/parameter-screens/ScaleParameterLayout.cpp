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
  m_masterButton = addControl(new Button("Master...", Buttons::BUTTON_A));
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
          ebUseCases.selectParameter({C15::PID::Master_Volume, VoiceGroup::Global}, true);
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
  static const auto ids = std::vector<int> { 312, 391, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323 };
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases { *eb };
  auto id = eb->getSelectedParameterNumber();

  auto idIt = std::find(ids.begin(), ids.end(), id);
  if(idIt == ids.end() || (*idIt == 323 && inc > 0))
  {
    id = 312;
  }
  else
  {
    auto toEnd = std::distance(idIt, ids.end());
    auto toBegin = std::distance(ids.begin(), idIt);

    if(inc > 0 && toEnd != 1)
      inc = std::min<int>(toEnd, inc);
    else if(inc > 0)
      idIt = ids.begin();

    if(inc < 0 && toBegin != 0)
      inc = std::min<int>(toBegin, inc);
    else if(inc < 0)
      idIt = ids.end();

    idIt += inc;
    id = *idIt;
  }

  ebUseCases.selectParameter({ id, VoiceGroup::Global }, true);
}
