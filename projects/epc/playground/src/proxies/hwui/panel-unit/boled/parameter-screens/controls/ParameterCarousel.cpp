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
  auto maxNumParameters = 4;
  if(buttonAssignments.size() <= maxNumParameters)
  {
    setupChildControlsThatFit(selectedParameter, buttonAssignments);
  }
  else if(ScaleGroup::isScaleParameter(selectedParameter))
  {
    setupChildControlsForScaleParameterCarousel(selectedParameter, buttonAssignments);
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
    auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
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

    if(ScaleGroup::isScaleParameter(param))
    {
      auto label = miniParam->getLabel();
      label->setInfix("...");
    }
  }
}

void ParameterCarousel::antiTurn()
{
  auto foundCtrl = std::dynamic_pointer_cast<MiniParameter>(*getControls().rbegin());
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases { *eb };
  for(const auto& ctrl : getControls())
  {
    if(auto p = std::dynamic_pointer_cast<MiniParameter>(ctrl))
    {
      if(p->isSelected())
      {
        ebUseCases.selectParameter(foundCtrl->getParameter()->getID(), true);
        return;
      }
      foundCtrl = p;
    }
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
    if(auto p = std::dynamic_pointer_cast<MiniParameter>(first()))
      ebUseCases.selectParameter(p->getParameter()->getID(), true);
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
      setupChildControls(selectedParameter,
                         { Master_Volume, Master_Tune, Master_Pan, Master_Serial_FX, Scale_Base_Key });
    else
      setupChildControls(selectedParameter, { Master_Volume, Master_Tune, Scale_Base_Key });
  }
  else if(paramID == Scale_Base_Key || ScaleGroup::isScaleParameter(selectedParameter))
  {
    if(isDualSound)
      setupChildControls(selectedParameter,
                         { Master_Serial_FX, Scale_Base_Key, Scale_Offset_0, Scale_Offset_1, Scale_Offset_2,
                           Scale_Offset_3, Scale_Offset_4, Scale_Offset_5, Scale_Offset_6, Scale_Offset_7,
                           Scale_Offset_8, Scale_Offset_9, Scale_Offset_10, Scale_Offset_11, Master_Volume });
    else
      setupChildControls(selectedParameter,
                         { Master_Tune, Scale_Base_Key, Scale_Offset_0, Scale_Offset_1, Scale_Offset_2, Scale_Offset_3,
                           Scale_Offset_4, Scale_Offset_5, Scale_Offset_6, Scale_Offset_7, Scale_Offset_8,
                           Scale_Offset_9, Scale_Offset_10, Scale_Offset_11, Master_Volume });
  }
  else if(paramID == Unison_Voices || paramID == Unison_Detune || paramID == Unison_Phase || paramID == Unison_Pan)
  {
    setupChildControls(selectedParameter, { Unison_Voices, Unison_Detune, Unison_Phase, Unison_Pan });
  }
}

void ParameterCarousel::setupChildControlsForScaleParameterCarousel(Parameter* selectedParameter,
                                                                    const std::list<int>& buttonAssignments)
{
  auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  const int ySpaceing = 3;
  const int miniParamHeight = 12;
  const int miniParamWidth = 56;
  auto yPos = ySpaceing;

  auto itOfSelectedParameter
      = std::find(buttonAssignments.begin(), buttonAssignments.end(), selectedParameter->getID().getNumber());
  auto itOfElementBefore = itOfSelectedParameter;
  std::advance(itOfElementBefore, -1);

  auto distanceToEnd = std::distance(itOfSelectedParameter, buttonAssignments.end());
  auto itOfLastShownElement = itOfSelectedParameter;
  std::advance(itOfLastShownElement, std::min(3l, distanceToEnd));

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
    decorateMiniParameterControlForScaleParameterCarousel(param, miniParam);

    yPos += ySpaceing;
    yPos += miniParamHeight;
  }
}

void ParameterCarousel::decorateMiniParameterControlForScaleParameterCarousel(const Parameter* param,
                                                                              MiniParameter* miniParam) const
{
  if(!ScaleGroup::isScaleParameter(param))
  {
    auto label = miniParam->getLabel();
    label->setInfix("...");
  }
  else
  {
    miniParam->getLabel()->setParameterNameTransformer([](auto in) { return StringTools::removeSpaces(in); });
  }
}

void ParameterCarousel::setupChildControlsThatDontFit(Parameter* selectedParameter,
                                                      const std::list<int>& buttonAssignments)
{
  auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
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

    if(ScaleGroup::isScaleParameter(param))
    {
      auto label = miniParam->getLabel();
      label->setInfix("...");
    }
  }
}
