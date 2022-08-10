#include "ParameterLayout.h"
#include "StaticKnubbelSlider.h"
#include "StaticBarSlider.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/ParameterUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include "groups/ScaleGroup.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/MCAmountButton.h"
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
#include <device-settings/HighlightChangedParametersSetting.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/controls/SwitchVoiceGroupButton.h>
#include <presets/recall/RecallParameter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <libundo/undo/Scope.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MuteIndicator.h>
#include <sigc++/adaptors/hide.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupIndicator.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterNotAvailableInSoundInfo.h>
#include <glibmm/main.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include <parameter_declarations.h>

ParameterLayout2::ParameterLayout2()
    : super(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
    , m_soundTypeRedrawThrottler { Application::get().getMainContext(), std::chrono::milliseconds(50) }
{
  addControl(new ParameterNameLabel(Rect(BIG_SLIDER_X - 2, 8, BIG_SLIDER_WIDTH + 4, 11)));
  addControl(new LockedIndicator(Rect(65, 1, 10, 11)));
  addControl(new VoiceGroupIndicator(Rect(2, 15, 16, 16), false));
  addControl(new UndoIndicator(Rect(22, 15, 10, 8)));
  addControl(new ParameterNotAvailableInSoundInfo(Rect(64, 0, 128, 48)));

  Application::get().getPresetManager()->getEditBuffer()->onSoundTypeChanged(
      sigc::hide(sigc::mem_fun(this, &ParameterLayout2::onSoundTypeChanged)), false);
}

ModuleCaption *ParameterLayout2::createModuleCaption() const
{
  return new ModuleCaption(Rect(0, 0, 64, 13));
}

void ParameterLayout2::init()
{
  addControl(createModuleCaption());
  showRecallScreenIfAppropriate();
}

void ParameterLayout2::copyFrom(Layout *src)
{
  super::copyFrom(src);
  showRecallScreenIfAppropriate();
}

void ParameterLayout2::showRecallScreenIfAppropriate()
{
  if(Application::get().getHWUI()->getButtonModifiers()[SHIFT])
  {
    handlePresetValueRecall();
  }
}

Parameter *ParameterLayout2::getCurrentParameter() const
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  return Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);
}

Parameter *ParameterLayout2::getCurrentEditParameter() const
{
  return getCurrentParameter();
}

bool ParameterLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(i == Buttons::BUTTON_SHIFT)
  {
    if(down)
    {
      handlePresetValueRecall();
    }
    return true;
  }

  if(down)
  {
    SettingsUseCases useCases(*Application::get().getSettings());

    switch(i)
    {
      case Buttons::BUTTON_PRESET:
        useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Select));
        return true;

      case Buttons::BUTTON_STORE:
        useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Store));
        return true;

      case Buttons::BUTTON_INFO:
        useCases.setFocusAndMode(FocusAndMode { UIMode::Info });
        return true;

      case Buttons::BUTTON_DEFAULT:
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
  {
    ParameterUseCases useCase(p);
    useCase.toggleLoadDefault();
  }
}

void ParameterLayout2::onSoundTypeChanged()
{
  m_soundTypeRedrawThrottler.doTask([&] {
    Application::get().getMainContext()->signal_idle().connect_once(sigc::mem_fun(this, &ParameterLayout2::setDirty));
  });
}

bool ParameterLayout2::onRotary(int inc, ButtonModifiers modifiers)
{
  if(auto p = getCurrentEditParameter())
  {
    ParameterUseCases useCase(p);
    useCase.incDec(inc, modifiers[ButtonModifier::FINE], modifiers[ButtonModifier::SHIFT]);
    return true;
  }

  return super::onRotary(inc, modifiers);
}

void ParameterLayout2::handlePresetValueRecall()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto editParam = getCurrentEditParameter())
  {
    if(editParam->isValueChangedFromLoaded())
    {
      if(editParam->getParentGroup()->getID().getName() == "Part" && eb->getType() == SoundType::Layer)
        getOLEDProxy().setOverlay(new PartMasterRecallLayout2());
      else
        getOLEDProxy().setOverlay(new ParameterRecallLayout2());
    }
  }
}

Control *ParameterLayout2::createParameterValueControl()
{
  return new SelectedParameterValue(Rect(90, 33, 76, 12));
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
  auto parameter = getCurrentParameter();
  if(!ScaleGroup::isScaleParameter(parameter))
  {
    return new ParameterCarousel(rect);
  }
  else
  {
    return new ScaleParameterCarousel(rect);
  }
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

bool ParameterSelectLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    switch(i)
    {
      case Buttons::BUTTON_A:
        if(auto button = findControlOfType<SwitchVoiceGroupButton>())
        {
          EditBufferUseCases ebUseCases(*getCurrentEditParameter()->getParentEditBuffer());
          if(SwitchVoiceGroupButton::allowToggling(getCurrentParameter(),
                                                   Application::get().getPresetManager()->getEditBuffer()))
            Application::get().getVGManager()->toggleCurrentVoiceGroupAndUpdateParameterSelection();
          else if(button->getText().text == "back..")
            ebUseCases.selectParameter({ C15::PID::Master_Volume, VoiceGroup::Global });
          return true;
        }
        break;

      case Buttons::BUTTON_D:
        if(m_carousel && !getCurrentParameter()->isDisabled())
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

      case Buttons::BUTTON_EDIT:
      {
        SettingsUseCases useCases(*Application::get().getSettings());
        useCases.setFocusAndMode(FocusAndMode { UIMode::Edit });
        return true;
      }
    }
  }

  return super::onButton(i, down, modifiers);
}

ParameterEditLayout2::ParameterEditLayout2()
    : super()
{
  addControl(new InvertedLabel("Edit", Rect(8, 26, 48, 12)))->setHighlight(true);
  remove(findControlOfType<VoiceGroupIndicator>().get());
  remove(findControlOfType<UndoIndicator>().get());
}

ParameterEditLayout2::~ParameterEditLayout2() = default;

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

bool ParameterEditLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    if(m_menu)
    {
      if(Buttons::BUTTON_D == i)
      {
        if(modifiers[SHIFT] == 1)
          m_menu->antiToggle();
        else
          m_menu->toggle();
        return true;
      }
      if(Buttons::BUTTON_ENTER == i)
      {
        m_menu->doAction();
        return true;
      }
    }

    if(Buttons::BUTTON_EDIT == i)
    {
      SettingsUseCases useCases(*Application::get().getSettings());
      useCases.setFocusAndMode(FocusAndMode { UIMode::Select });
      return true;
    }
  }

  return super::onButton(i, down, modifiers);
}

ParameterRecallLayout2::ParameterRecallLayout2()
    : super()
{
  Application::get().getSettings()->getSetting<ForceHighlightChangedParametersSetting>()->set(
      BooleanSetting::tEnum::BOOLEAN_SETTING_TRUE);

  m_buttonA = addControl(new Button("", Buttons::BUTTON_A));
  m_buttonB = addControl(new Button("", Buttons::BUTTON_B));
  m_buttonC = addControl(new Button("", Buttons::BUTTON_C));
  m_buttonD = addControl(new Button("", Buttons::BUTTON_D));

  if(auto p = getCurrentParameter())
  {
    auto originalParam = p->getOriginalParameter();
    auto originalValue = originalParam ? originalParam->getRecallValue() : p->getDefaultValue();

    switch(p->getVisualizationStyle())
    {
      case Parameter::VisualizationStyle::Bar:
        m_slider = addControl(
            new StaticBarSlider(originalValue, p->isBiPolar(), Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
      case Parameter::VisualizationStyle::BarFromRight:
        m_slider
            = addControl(new StaticDrawFromRightBarSlider(originalValue, Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
      case Parameter::VisualizationStyle::Dot:
        m_slider = addControl(
            new StaticKnubbelSlider(originalValue, p->isBiPolar(), Rect(BIG_SLIDER_X, 24, BIG_SLIDER_WIDTH, 6)));
        break;
    }

    m_leftValue = addControl(new Label(StringAndSuffix { p->getDisplayString() }, Rect(67, 35, 58, 11)));

    auto displayString = p->getDisplayString(originalValue);

    m_rightValue = addControl(new Label(StringAndSuffix { displayString }, Rect(131, 35, 58, 11)));
  }

  m_recallValue = getCurrentParameter()->getControlPositionValue();

  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &ParameterRecallLayout2::onParameterSelectionChanged), vg);

  updateUI(false);
}

ParameterRecallLayout2::~ParameterRecallLayout2()
{
  Application::get().getSettings()->getSetting<ForceHighlightChangedParametersSetting>()->set(
      BooleanSetting::tEnum::BOOLEAN_SETTING_FALSE);
}

void ParameterRecallLayout2::init()
{
  addControl(createModuleCaption());
}

bool ParameterRecallLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && m_paramLikeInPreset && i == Buttons::BUTTON_C)
    undoRecall();
  else if(down && !m_paramLikeInPreset && i == Buttons::BUTTON_B)
    doRecall();

  if(i == Buttons::BUTTON_C || i == Buttons::BUTTON_B)
    return true;

  if(i == Buttons::BUTTON_INC || i == Buttons::BUTTON_DEC)
  {
    auto resetOled = (i == Buttons::BUTTON_INC && !getCurrentParameter()->isMaximum())
        || (i == Buttons::BUTTON_DEC && !getCurrentParameter()->isMinimum());

    auto ret = super::onButton(i, down, modifiers);
    if(resetOled)
      getOLEDProxy().resetOverlay();
    return ret;
  }

  if(i == Buttons::BUTTON_PRESET)
  {
    if(down)
    {
      SettingsUseCases useCases(*Application::get().getSettings());
      useCases.setFocusAndMode(FocusAndMode { UIFocus::Presets, UIMode::Select });
    }
    return true;
  }

  if(i == Buttons::BUTTON_SHIFT && !down)
    getOLEDProxy().resetOverlay();

  return false;
}

bool ParameterRecallLayout2::onRotary(int inc, ButtonModifiers modifiers)
{
  auto resetScreen
      = (inc > 0 && !getCurrentParameter()->isMaximum()) || (inc < 0 && !getCurrentParameter()->isMinimum());
  auto ret = super::onRotary(inc, modifiers);

  if(resetScreen)
    getOLEDProxy().resetOverlay();
  return ret;
}

void ParameterRecallLayout2::doRecall()
{
  if(auto curr = getCurrentParameter())
  {
    m_recallString = curr->getDisplayString();
    m_recallValue = curr->getControlPositionValue();
    ParameterUseCases useCase(curr);
    useCase.recallParameterFromPreset();
    updateUI(true);
  }
}

void ParameterRecallLayout2::undoRecall()
{
  if(auto curr = getCurrentParameter())
  {
    ParameterUseCases useCase(curr);
    useCase.undoRecallParameterFromPreset(m_recallValue);
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
    if(paramLikeInPreset)
    {
      m_leftValue->setText(StringAndSuffix { p->getDisplayString() });
      m_rightValue->setText(StringAndSuffix { m_recallString });
      m_slider->setValue(p->getControlPositionValue(), p->isBiPolar());
      m_rightValue->setHighlight(false);
      m_leftValue->setHighlight(true);
      m_buttonB->setText(StringAndSuffix::empty());
      m_buttonC->setText(StringAndSuffix { "Recall" });
    }
    else
    {
      auto originalParam = p->getOriginalParameter();
      auto originalValue = originalParam ? originalParam->getRecallValue() : p->getDefaultValue();
      auto displayString = p->getDisplayString(originalValue);

      m_leftValue->setText(StringAndSuffix { displayString });
      m_rightValue->setText(StringAndSuffix { p->getDisplayString() });
      m_slider->setValue(m_recallValue, p->isBiPolar());
      m_leftValue->setHighlight(false);
      m_rightValue->setHighlight(true);
      m_buttonC->setText(StringAndSuffix::empty());
      m_buttonB->setText(StringAndSuffix { "Recall" });
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

PartMasterRecallLayout2::PartMasterRecallLayout2()
    : ParameterRecallLayout2()
    , m_muteParameter { Application::get().getPresetManager()->getEditBuffer()->findParameterByID(
          { C15::PID::Voice_Grp_Mute, Application::get().getVGManager()->getCurrentVoiceGroup() }) }
{
  m_muteParameterConnection
      = m_muteParameter->onParameterChanged(sigc::hide(sigc::mem_fun(this, &PartMasterRecallLayout2::onMuteChanged)));
}

PartMasterRecallLayout2::~PartMasterRecallLayout2()
{
  m_muteParameterConnection.disconnect();
}

void PartMasterRecallLayout2::updateUI(bool paramLikeInPreset)
{
  ParameterRecallLayout2::updateUI(paramLikeInPreset);

  if(!shouldShowNormalRecallScreen())
  {
    m_buttonB->setText(StringAndSuffix::empty());
    m_buttonC->setText(StringAndSuffix::empty());
    m_leftValue->setText(StringAndSuffix::empty());
    m_rightValue->setText(StringAndSuffix::empty());
  }
}

bool PartMasterRecallLayout2::shouldShowNormalRecallScreen() const
{
  return getCurrentParameter()->isChangedFromLoaded() || !m_recallString.empty();
}

bool PartMasterRecallLayout2::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(shouldShowNormalRecallScreen())
  {
    if(ParameterRecallLayout2::onButton(i, down, modifiers))
      return true;
  }
  else
  {
    if(i == Buttons::BUTTON_SHIFT && !down)
    {
      getOLEDProxy().resetOverlay();
      return true;
    }
  }

  if(i == Buttons::BUTTON_A && down)
  {
    toggleMute();
    return true;
  }

  return false;
}

void PartMasterRecallLayout2::toggleMute() const
{
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  EditBufferUseCases useCase(*eb);
  useCase.toggleMute(m_muteParameter->getID().getVoiceGroup());
}

void PartMasterRecallLayout2::onMuteChanged()
{
  if(m_muteParameter->getControlPositionValue() > 0.5)
    m_buttonA->setText(StringAndSuffix { "Unmute" });
  else
    m_buttonA->setText(StringAndSuffix { "Mute" });
}
