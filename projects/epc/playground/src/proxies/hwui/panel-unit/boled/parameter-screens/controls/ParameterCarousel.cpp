#include <proxies/hwui/panel-unit/PanelUnitParameterEditMode.h>
#include <groups/ScaleGroup.h>
#include <parameters/ScaleParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include "ParameterCarousel.h"
#include "Application.h"
#include "proxies/hwui/HWUI.h"
#include "parameters/Parameter.h"
#include "MiniParameter.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "proxies/hwui/controls/Button.h"
#include "NeverHighlitButton.h"
#include "use-cases/EditBufferUseCases.h"
#include "parameter_declarations.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/MiniParameterLabel.h"
#include "MiniParameterBarSlider.h"

ParameterCarousel::ParameterCarousel(const Rect& pos)
    : super(pos)
{
}

ParameterCarousel::~ParameterCarousel()
{
}

void ParameterCarousel::setup(Parameter* selectedParameter)
{
  clear();

  auto um = Application::get().getHWUI()->getPanelUnit().getUsageMode();
  const auto paramAvailable = !selectedParameter->isDisabled();

  if(std::shared_ptr<PanelUnitParameterEditMode> edit = std::dynamic_pointer_cast<PanelUnitParameterEditMode>(um))
  {
    if(paramAvailable)
    {
      auto button = edit->findButtonForParameter(selectedParameter);

      if(button != Buttons::INVALID && static_cast<int>(button) != -1)
      {
        setupChildControls(edit, selectedParameter, button);
      }
      else
      {
        setupChildControlsForParameterWithoutButtonMapping(selectedParameter);
      }
    }
  }

  if(getNumChildren() == 0)
  {
    addControl(new NeverHighlitButton("", Rect(0, 51, 58, 11)));
  }
  else
  {
    setHighlight(true);
  }

  setDirty();
}

void ParameterCarousel::setupChildControls(const std::shared_ptr<PanelUnitParameterEditMode>& edit,
                                           Parameter* selectedParameter, Buttons button)
{
  std::list<int> buttonAssignments
      = edit->getButtonAssignments(button, Application::get().getPresetManager()->getEditBuffer()->getType());

  if(buttonAssignments.size() > 1)
  {
    setupChildControls(selectedParameter, buttonAssignments);
  }
}

void ParameterCarousel::setupChildControls(Parameter* selectedParameter, const std::list<int>& buttonAssignments)
{
  m_currentCarouselContentIDs = { buttonAssignments.begin(), buttonAssignments.end() };

  auto maxNumParameters = 4;
  if(buttonAssignments.size() <= maxNumParameters)
  {
    setupChildControlsThatFit(selectedParameter, buttonAssignments);
  }
  else
  {
    setupChildControlsThatDontFit(selectedParameter, buttonAssignments);
  }
}

void ParameterCarousel::setupChildControlsThatFit(Parameter* selectedParameter, const std::list<int>& buttonAssignments)
{
  const auto maxNumParameters = 4;
  const int ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  auto yPos = ySpaceing;
  auto missingParams = maxNumParameters - buttonAssignments.size();
  yPos += missingParams * (miniParamHeight + ySpaceing);

  for(int i : buttonAssignments)
  {
    auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
    auto eb = Application::get().getPresetManager()->getEditBuffer();

    auto param = eb->findParameterByID({ i, vg });

    if(!param)
      param = eb->findParameterByID({ i, VoiceGroup::Global });

    if(!param)
      continue;

    auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));

    if(dynamic_cast<ScaleParameter*>(selectedParameter) != nullptr)
    {
      miniParam->setSelected(param->getID().getNumber() == ScaleGroup::getScaleBaseParameterNumber());
    }
    else
    {
      miniParam->setSelected(param->getID().getNumber() == selectedParameter->getID().getNumber());
    }

    addControl(miniParam);
    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}

void ParameterCarousel::antiTurn()
{
  bool found = false;
  bool handled = false;
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases { *eb };

  tIfCallback cb = ([&](const tControlPtr& ctrl) -> bool {
                      if(auto p = std::dynamic_pointer_cast<MiniParameter>(ctrl))
                      {
                        if(found)
                        {
                          ebUseCases.selectParameter(p->getParameter()->getID(), true);
                          handled = true;
                          return false;
                        }

                        if(p->isSelected())
                        {
                          found = true;
                        }
                      }

                      return true;
                    });

  forEachReversed(cb);

  if(!handled)
  {
    auto idToSelect = lastParameterIDOfCarousel();
    auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
    if(ParameterId::isGlobal(idToSelect))
      ebUseCases.selectParameter({ idToSelect, VoiceGroup::Global }, true);
    else
      ebUseCases.selectParameter({ idToSelect, vg }, true);
  }
}

void ParameterCarousel::turn()
{
  bool found = false;
  bool handled = false;
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases { *eb };

  tIfCallback cb = ([&](const tControlPtr& ctrl) -> bool {
    if(auto p = std::dynamic_pointer_cast<MiniParameter>(ctrl))
    {
      if(found)
      {
        ebUseCases.selectParameter(p->getParameter()->getID(), true);
        handled = true;
        return false;
      }

      if(p->isSelected())
      {
        found = true;
      }
    }

    return true;
  });

  forEach(cb);

  if(!handled)
  {
    auto idToSelect = firstParameterIDOfCarousel();
    auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
    if(ParameterId::isGlobal(idToSelect))
      ebUseCases.selectParameter({ idToSelect, VoiceGroup::Global }, true);
    else
      ebUseCases.selectParameter({ idToSelect, vg }, true);
  }
}

void ParameterCarousel::setupChildControlsForParameterWithoutButtonMapping(Parameter* selectedParameter)
{
  using namespace C15::PID;
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto soundType = eb->getType();
  const auto isDualSound = soundType == SoundType::Layer || soundType == SoundType::Split;
  const auto paramID = selectedParameter->getID().getNumber();

  if(paramID == Master_Volume || paramID == Master_Tune || paramID == Master_Pan || paramID == Master_Serial_FX)
  {
    if(isDualSound)
      setupChildControls(selectedParameter, { Master_Volume, Master_Tune, Master_Pan, Master_Serial_FX });
    else
      setupChildControls(selectedParameter, { Master_Volume, Master_Tune });
  }
  else if(paramID == Scale_Base_Key || ScaleGroup::isScaleParameter(selectedParameter))
  {
    setupChildControls(selectedParameter,
                       { Scale_Base_Key, Scale_Offset_0, Scale_Offset_1, Scale_Offset_2, Scale_Offset_3, Scale_Offset_4,
                         Scale_Offset_5, Scale_Offset_6, Scale_Offset_7, Scale_Offset_8, Scale_Offset_9,
                         Scale_Offset_10, Scale_Offset_11 });
  }
  else if(paramID == Unison_Voices || paramID == Unison_Detune || paramID == Unison_Phase || paramID == Unison_Pan)
  {
    setupChildControls(selectedParameter, { Unison_Voices, Unison_Detune, Unison_Phase, Unison_Pan });
  }
}

void ParameterCarousel::setupChildControlsThatDontFit(Parameter* selectedParameter,
                                                      const std::list<int>& buttonAssignments)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  const int ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  auto yPos = ySpaceing;

  auto itOfSelectedParameter
      = std::find(buttonAssignments.begin(), buttonAssignments.end(), selectedParameter->getID().getNumber());
  auto itOfElementBefore = itOfSelectedParameter;
  if(itOfSelectedParameter != buttonAssignments.begin())
    std::advance(itOfElementBefore, -1);

  auto distanceToEnd = std::distance(itOfSelectedParameter, buttonAssignments.end());
  auto itOfLastShownElement = itOfSelectedParameter;

  if(itOfSelectedParameter != buttonAssignments.begin())
    std::advance(itOfLastShownElement, std::min(3l, distanceToEnd));
  else
    std::advance(itOfLastShownElement, std::min(4l, distanceToEnd));

  for(auto it = itOfElementBefore; it != itOfLastShownElement; it++)
  {
    auto i = *it;
    auto param = eb->findParameterByID({ i, vg });

    if(!param)
      param = eb->findParameterByID({ i, VoiceGroup::Global });

    if(!param)
      continue;

    auto miniParam = new MiniParameter(param, Rect(0, yPos, miniParamWidth, miniParamHeight));
    addControl(miniParam);
    miniParam->setSelected(it == itOfSelectedParameter);

    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}

int ParameterCarousel::firstParameterIDOfCarousel() const
{
  return m_currentCarouselContentIDs.front();
}

int ParameterCarousel::lastParameterIDOfCarousel() const
{
  return m_currentCarouselContentIDs.back();
}

ScaleParameterCarousel::ScaleParameterCarousel(const Rect& r)
    : Carousel(r)
{
}

ScaleParameterCarousel::~ScaleParameterCarousel()
{
}

void ScaleParameterCarousel::setup(Parameter* selectedParameter)
{
  using namespace C15::PID;

  const int ySpaceing = 9;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  auto yPos = ySpaceing;

  clear();
  auto eb = selectedParameter->getParentEditBuffer();
  auto baseParameter = eb->findParameterByID({ Scale_Base_Key, VoiceGroup::Global });
  std::vector<Parameter*> offsets;
  for(auto i : { Scale_Offset_0, Scale_Offset_1, Scale_Offset_2, Scale_Offset_3, Scale_Offset_4, Scale_Offset_5,
                 Scale_Offset_6, Scale_Offset_7, Scale_Offset_8, Scale_Offset_9, Scale_Offset_10, Scale_Offset_11 })
  {
    offsets.emplace_back(eb->findParameterByID({ i, VoiceGroup::Global }));
  }

  auto baseParam = new MiniParameter(baseParameter, Rect(0, yPos, miniParamWidth, miniParamHeight));
  addControl(baseParam);
  baseParam->setSelected(selectedParameter == baseParameter);

  const int sliderHeight = 3;
  constexpr int paramWidth = 27;
  constexpr auto offsetYOffset = miniParamHeight + ySpaceing + 3;
  yPos = offsetYOffset;

  auto idx = 0;
  for(auto o : offsets)
  {
    auto rowIDX = (idx < 6 ? 0 : 1);
    if(idx == 6)
      yPos = offsetYOffset;

    auto isLowerGroup = [](auto idx)
    {
      auto lowerGroupIDs = { 3, 4, 5, 9, 10, 11 };
      auto it = std::find(lowerGroupIDs.begin(), lowerGroupIDs.end(), idx);
      return it != lowerGroupIDs.end();
    };

    auto yOffset = 0;
    if(isLowerGroup(idx))
    {
      yOffset = 5;
    }

    auto xPos = (paramWidth * rowIDX) + (rowIDX * 2);

    auto miniParam = new MiniParameterBarSlider(o, Rect(xPos, yPos + yOffset, paramWidth, sliderHeight));
    addControl(miniParam);
    miniParam->setHighlight(o == selectedParameter);
    yPos += (sliderHeight * 2);
    idx++;
  }
}

void ScaleParameterCarousel::turn()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto selectedParameterNumber = eb->getSelectedParameterNumber();
  using namespace C15::PID;
  std::vector<int> order = { Scale_Base_Key, Scale_Offset_0,  Scale_Offset_1, Scale_Offset_2, Scale_Offset_3,
                             Scale_Offset_4, Scale_Offset_5,  Scale_Offset_6, Scale_Offset_7, Scale_Offset_8,
                             Scale_Offset_9, Scale_Offset_10, Scale_Offset_11 };

  auto selectedIt = std::find(order.begin(), order.end(), selectedParameterNumber);
  EditBufferUseCases ebUseCases(*eb);
  if(selectedIt != order.end())
  {
    if(*selectedIt == order.back())
      selectedIt = order.begin();
    else
      selectedIt = std::next(selectedIt);

    auto id = ParameterId({*selectedIt, VoiceGroup::Global});
    ebUseCases.selectParameter(id, true);
  }
  else
  {
    ebUseCases.selectParameter({order.front(), VoiceGroup::Global}, true);
  }
}

void ScaleParameterCarousel::antiTurn()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto selectedParameterNumber = eb->getSelectedParameterNumber();
  using namespace C15::PID;
  std::vector<int> order = { Scale_Base_Key, Scale_Offset_0,  Scale_Offset_1, Scale_Offset_2, Scale_Offset_3,
                             Scale_Offset_4, Scale_Offset_5,  Scale_Offset_6, Scale_Offset_7, Scale_Offset_8,
                             Scale_Offset_9, Scale_Offset_10, Scale_Offset_11 };

  auto selectedIt = std::find(order.begin(), order.end(), selectedParameterNumber);
  EditBufferUseCases ebUseCases(*eb);
  if(selectedIt != order.end())
  {
    if(*selectedIt == order.front())
      selectedIt = std::prev(order.end());
    else
      selectedIt = std::prev(selectedIt);

    auto id = ParameterId{*selectedIt, VoiceGroup::Global};
    ebUseCases.selectParameter(id, true);
  }
  else
  {
    ebUseCases.selectParameter({order.back(), VoiceGroup::Global}, true);
  }
}
