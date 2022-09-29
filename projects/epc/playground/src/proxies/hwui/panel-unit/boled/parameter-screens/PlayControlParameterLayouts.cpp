#include "PlayControlParameterLayouts.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "use-cases/RibbonParameterUseCases.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterKnubbelSlider.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterBarSlider.h"
#include "proxies/hwui/controls/SwitchVoiceGroupButton.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/PhysicalControlSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationRoutersCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/PhysicalControlBehaviorLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNameLabel.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/ParameterAlgorithm.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/RibbonParameter.h>
#include <groups/HardwareSourcesGroup.h>
#include <groups/MacroControlMappingGroup.h>
#include <device-settings/RibbonRelativeFactor.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterEditButtonMenu.h>
#include <libundo/undo/Scope.h>

uint8_t PlayControlParameterLayout2::s_mode = PlayControlParameterLayout2::Mode::ValueEdit;
int PlayControlParameterLayout2::s_instanceCount = 0;

PlayControlParameterLayout2::PlayControlParameterLayout2()
    : super()
{
  addControl(new PhysicalControlSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
  addControl(new HardwareSourceCCLabel(Rect{10, 33, 45, 12}));

  if(s_instanceCount == 0)
  {
    s_mode = Mode::ValueEdit;
  }

  setMode(s_mode);

  addControl(new Button("MC", Buttons::BUTTON_A));

  auto selectButton = addControl(new Button("Select", Buttons::BUTTON_B));
  selectButton->setHighlight(s_mode == Mode::Select);

  auto behaviourButton = addControl(new Button("Behaviour", Buttons::BUTTON_C));
  behaviourButton->setHighlight(s_mode == Mode::Behaviour);

  auto modeButton = addControl(new Button("Mode", Buttons::BUTTON_D));
  modeButton->setHighlight(s_mode == Mode::Mode);

  if(!supportsMode())
    modeButton->setText("");

  if(!supportsBehaviour())
    behaviourButton->setText("");

  s_instanceCount++;
}

Control *PlayControlParameterLayout2::createParameterValueControl()
{
  switch(s_mode)
  {
    default:
    case Mode::ValueEdit:
    case Mode::Select:
      return new PhysicalControlValueLabel(Rect(70, 33, 116, 12));
    case Mode::Behaviour:
      return new PhysicalControlBehaviorLabel(Rect(64 * 2, 33, 64, 12));
    case Mode::Mode:
      return new PhysicalControlModeLabel(Rect(64 * 3, 33, 64, 12));
  }
}

void PlayControlParameterLayout2::onInit()
{
  setMode(s_mode);
}

PlayControlParameterLayout2::~PlayControlParameterLayout2()
{
  s_instanceCount--;
}

bool PlayControlParameterLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && Buttons::BUTTON_A == i)
  {
    EditBufferUseCases ebUseCases(*getCurrentParameter()->getParentEditBuffer());
    ebUseCases.selectLastSelectedMacroControlParameter();
  }

  if(down && Buttons::BUTTON_B == i)
  {
    toggleMode(Mode::Select);
    return true;
  }

  if(down && Buttons::BUTTON_C == i)
  {
    toggleMode(Mode::Behaviour);
  }

  if(down && Buttons::BUTTON_D == i)
  {
    toggleMode(Mode::Mode);
  }

  if(down && Buttons::BUTTON_EDIT == i)
  {
    SettingsUseCases useCases(*Application::get().getSettings());
    useCases.setFocusAndMode(FocusAndMode { UIMode::Edit });
    return true;
  }

  return super::onButton(i, down, modifiers);
}

bool PlayControlParameterLayout2::onRotary(int i, ButtonModifiers modifiers)
{
  if(s_mode == Mode::Select)
  {
    auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
    EditBufferUseCases ebUseCases(*editBuffer);
    auto hw = dynamic_cast<HardwareSourcesGroup *>(editBuffer->getParameterGroupByID({ "CS", VoiceGroup::Global }));
    if(isSendParameter())
    {
      auto sendParam = dynamic_cast<const HardwareSourceSendParameter *>(getCurrentParameter());
      auto newParamID = getIdOfAdvancedParameter(hw->getSendParameters(), sendParam->getID(), i);
      ebUseCases.selectParameter(newParamID, true);
    }
    else
    {
      auto currentID = getCurrentParameter()->getID();
      auto newParamID = getIdOfAdvancedParameter(hw->getPhysicalControlParameters(), currentID, i);
      ebUseCases.selectParameter(newParamID, true);
    }
    return true;
  }

  if(s_mode == Mode::ValueEdit)
  {
    ParameterUseCases useCase(getCurrentParameter());
    useCase.incDec(i, modifiers[ButtonModifier::FINE], modifiers[ButtonModifier::SHIFT]);
    return true;
  }

  if(s_mode == Mode::Behaviour)
  {
    auto hw = dynamic_cast<PhysicalControlParameter *>(getCurrentParameter());

    if(hw)
    {
      PhysicalControlParameterUseCases useCase(hw);
      useCase.stepBehaviour(i);
    }
    else if(auto send = dynamic_cast<HardwareSourceSendParameter *>(getCurrentParameter()))
    {
      PhysicalControlParameterUseCases useCase(send->getSiblingParameter());
      useCase.stepBehaviour(i);
    }
    return true;
  }

  if(s_mode == Mode::Mode)
  {
    if(auto ribbonHW = dynamic_cast<RibbonParameter *>(getCurrentParameter()))
    {
      RibbonParameterUseCases ribbonUseCases(ribbonHW);
      ribbonUseCases.stepTouchBehaviour(i);
    }
    else if(auto send = dynamic_cast<HardwareSourceSendParameter *>(getCurrentParameter()))
    {
      if(auto ribbon = dynamic_cast<RibbonParameter *>(send->getSiblingParameter()))
      {
        RibbonParameterUseCases ribbonUseCases(ribbon);
        ribbonUseCases.stepTouchBehaviour(i);
      }
    }
    return true;
  }

  return super::onRotary(i, modifiers);
}

void PlayControlParameterLayout2::toggleMode(uint8_t desiredMode)
{
  if(isModeSupported(desiredMode))
  {
    if(s_mode == desiredMode)
      setMode(Mode::ValueEdit);
    else
      setMode(desiredMode);
  }
}

bool PlayControlParameterLayout2::isModeSupported(uint8_t desiredMode) const
{
  if(desiredMode == Mode::Behaviour)
    return supportsBehaviour();

  if(desiredMode == Mode::Mode)
    return supportsMode();

  return desiredMode == Mode::ValueEdit || desiredMode == Mode::Select;
}

uint8_t PlayControlParameterLayout2::getMode() const
{
  return s_mode;
}

void PlayControlParameterLayout2::setMode(uint8_t desiredMode)
{
  if(!isModeSupported(desiredMode))
    return;

  s_mode = desiredMode;

  noHighlight();
  setDirty();

  if(m_currentValueDisplay)
  {
    remove(m_currentValueDisplay);
    m_currentValueDisplay = nullptr;
  }

  m_currentValueDisplay = addControl(createParameterValueControl());

  switch(s_mode)
  {
    case Mode::ValueEdit:
    default:
      highlight<ParameterNameLabel>();
      highlight<PhysicalControlSlider>();
      highlight<SelectedParameterValue>();
      highlight<PhysicalControlValueLabel>();
      break;

    case Mode::Select:
      highlightButtonWithCaption("Select");
      break;

    case Mode::Behaviour:
      highlightButtonWithCaption("Behaviour");
      highlight<PhysicalControlBehaviorLabel>();
      break;

    case Mode::Mode:
      highlightButtonWithCaption("Mode");
      highlight<PhysicalControlModeLabel>();
  }
}

bool PlayControlParameterLayout2::isSendParameter() const
{
  return dynamic_cast<const HardwareSourceSendParameter *>(getCurrentParameter());
}

bool PlayControlParameterLayout2::supportsBehaviour() const
{
  auto ribbonParams = { C15::PID::Ribbon_1, C15::PID::Ribbon_2, C15::PID::Ribbon_3, C15::PID::Ribbon_4 };
  auto pedalParams = { C15::PID::Pedal_1, C15::PID::Pedal_2, C15::PID::Pedal_3, C15::PID::Pedal_4 };
  auto currentParamId = getCurrentParameter()->getID().getNumber();
  auto isRibbonParam = std::find(ribbonParams.begin(), ribbonParams.end(), currentParamId) != ribbonParams.end();
  auto isPedalParam = std::find(pedalParams.begin(), pedalParams.end(), currentParamId) != pedalParams.end();
  return isRibbonParam || isPedalParam;
}

bool PlayControlParameterLayout2::supportsMode() const
{
  auto ribbonParams = { C15::PID::Ribbon_1, C15::PID::Ribbon_2, C15::PID::Ribbon_3, C15::PID::Ribbon_4 };
  auto currentParamId = getCurrentParameter()->getID().getNumber();
  auto isRibbonParam = std::find(ribbonParams.begin(), ribbonParams.end(), currentParamId) != ribbonParams.end();
  return isRibbonParam;
}

//edit mode!

PlayControlParameterEditLayout2::PlayControlParameterEditLayout2()
{
}

void PlayControlParameterEditLayout2::init()
{
  super1::init();
  super2::init();

  if(auto p = getCurrentParameter())
  {
    switch(p->getVisualizationStyle())
    {
      case Parameter::VisualizationStyle::Dot:
        addControl(new SelectedParameterKnubbelSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
      case Parameter::VisualizationStyle::Bar:
      case Parameter::VisualizationStyle::BarFromRight:
        addControl(new SelectedParameterBarSlider(Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
    }
  }

  addControl(createParameterValueControl());

  highlight<SelectedParameterBarSlider>();
  highlight<SelectedParameterValue>();
  highlight<ParameterNameLabel>();

  if(auto vgButton = findControlOfType<SwitchVoiceGroupButton>())
  {
    remove(vgButton.get());
    addControl(new Button("", Buttons::BUTTON_A));
  }
}

ButtonMenu *PlayControlParameterEditLayout2::createMenu(const Rect &rect)
{
  return new ParameterEditButtonMenu(rect);
}

bool PlayControlParameterEditLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(super1::onButton(i, down, modifiers))
    return true;

  return super2::onButton(i, down, modifiers);
}
