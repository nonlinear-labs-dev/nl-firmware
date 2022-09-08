#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <parameters/Parameter.h>
#include <parameters/MacroControlParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/panel-unit/PanelUnitPresetMode.h>
#include <proxies/hwui/TwoStateLED.h>
#include <memory>
#include "PanelUnitPresetMode.h"
#include "use-cases/SettingsUseCases.h"
#include <device-settings/HighlightChangedParametersSetting.h>
#include <sigc++/sigc++.h>
#include <device-settings/Settings.h>
#include <parameter_list.h>

PanelUnitPresetMode::PanelUnitPresetMode()
    : m_bruteForceLedThrottler(Application::get().getMainContext(), std::chrono::milliseconds(40))
{
  DebugLevel::gassy(__PRETTY_FUNCTION__);

  Application::get().getHWUI()->onModifiersChanged(
      sigc::hide(sigc::mem_fun(this, &PanelUnitPresetMode::bruteForceUpdateLeds)));

  Application::get().getPresetManager()->getEditBuffer()->onChange(
      mem_fun(this, &PanelUnitPresetMode::bruteForceUpdateLeds));

  Application::get().getVGManager()->onCurrentVoiceGroupChanged(
      sigc::hide(sigc::mem_fun(this, &PanelUnitPresetMode::bruteForceUpdateLeds)));
}

PanelUnitPresetMode::~PanelUnitPresetMode()
{
  Application::get().getSettings()->getSetting<ForceHighlightChangedParametersSetting>()->set(
      BooleanSetting::tEnum::BOOLEAN_SETTING_FALSE);
  DebugLevel::gassy(__PRETTY_FUNCTION__);
}

void PanelUnitPresetMode::bruteForceUpdateLeds()
{
  m_bruteForceLedThrottler.doTask(
      [this]()
      {
        if(Application::get().getHWUI()->getPanelUnit().getUsageMode().get() != this)
          return;

        std::array<TwoStateLED::LedState, numLeds> states { TwoStateLED::OFF };

        if(Application::get().getHWUI()->getButtonModifiers()[SHIFT] == true)
          getMappings().forEachButton(
              [&](Buttons buttonId, const std::list<int>& parameters)
              {
                Application::get().getSettings()->getSetting<ForceHighlightChangedParametersSetting>()->set(
                    BooleanSetting::tEnum::BOOLEAN_SETTING_TRUE);
                letChangedButtonsBlink(buttonId, parameters, states);
              });
        else
          getMappings().forEachButton(
              [&](Buttons buttonId, const std::list<int>& parameters)
              {
                Application::get().getSettings()->getSetting<ForceHighlightChangedParametersSetting>()->set(
                    BooleanSetting::tEnum::BOOLEAN_SETTING_FALSE);
                setStateForButton(buttonId, parameters, states);
              });

        applyStateToLeds(states);
      });
}

void PanelUnitPresetMode::letChangedButtonsBlink(Buttons buttonId, const std::list<int>& parameters,
                                                 std::array<TwoStateLED::LedState, numLeds>& states)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto currentParams = getMappings().findParameters(buttonId);
  auto ebParameters = editBuffer->getParametersSortedByNumber(vg);
  auto globalParameters = editBuffer->getParametersSortedByNumber(VoiceGroup::Global);

  bool anyChanged = false;
  for(const auto paramID : currentParams)
  {
    try
    {
      anyChanged |= ebParameters.at(paramID)->isChangedFromLoaded();
    }
    catch(...)
    {
      anyChanged |= globalParameters.at(paramID)->isChangedFromLoaded();
    }
  }
  states[(int) buttonId] = anyChanged ? TwoStateLED::BLINK : TwoStateLED::OFF;
}

void PanelUnitPresetMode::setStateForButton(Buttons buttonId, const std::list<int>& parameters,
                                            std::array<TwoStateLED::LedState, numLeds>& states)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto db = editBuffer->getParameterDB();
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();

  for(const auto i : parameters)
  {
    auto signalFlowIndicator = editBuffer->getParameterDB().getSignalPathIndication(i);

    Parameter* parameter = nullptr;

    parameter = editBuffer->findParameterByID({ i, vg });

    if(!parameter)
      parameter = editBuffer->findParameterByID({ i, VoiceGroup::Global });

    if(parameter != nullptr)
    {
      auto result = trySpecialCaseParameter(parameter);
      auto isSpecialCase = result.first;
      auto enabledLed = result.second;
      if(isSpecialCase)
      {
        if(enabledLed)
        {
          states[(int) buttonId] = TwoStateLED::ON;
          break;
        }
      }
      else if(auto mc = dynamic_cast<MacroControlParameter*>(parameter))
      {
        if(!mc->getTargets().empty())
        {
          states[(int) buttonId] = TwoStateLED::ON;
          break;
        }
      }
      else if(signalFlowIndicator != ParameterDB::getInvalidSignalPathIndication())
      {
        if(parameter->getControlPositionValue() != signalFlowIndicator)
        {
          states[(int) buttonId] = TwoStateLED::ON;
          break;
        }
      }
    }
  }
}

void PanelUnitPresetMode::applyStateToLeds(std::array<TwoStateLED::LedState, numLeds>& states)
{
  auto& panelUnit = Application::get().getHWUI()->getPanelUnit();
  for(int i = 0; i < numLeds; i++)
  {
    panelUnit.getLED((Buttons) i)->setState(states[i]);
  }
}

std::pair<bool, bool> PanelUnitPresetMode::trySpecialCaseParameter(const Parameter* selParam) const
{
  using namespace C15::PID;
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(selParam)
  {
    switch(selParam->getID().getNumber())
    {
      case Env_A_Sus:
      {
        auto splitLevel = eb->findParameterByID({ Env_A_Elevate, selParam->getVoiceGroup() });
        return { true, splitLevel->getControlPositionValue() != 0 || selParam->getControlPositionValue() > 0 };
      }
      case Env_B_Sus:
      {
        auto splitLevel = eb->findParameterByID({ Env_B_Elevate, selParam->getVoiceGroup() });
        return { true, splitLevel->getControlPositionValue() != 0 || selParam->getControlPositionValue() > 0 };
      }
      case Flanger_Mix:
      {
        auto tremolo = eb->findParameterByID({ Flanger_Tremolo, selParam->getVoiceGroup() });
        return { true, tremolo->getControlPositionValue() > 0 || selParam->getControlPositionValue() != 0 };
      }
      case Echo_Mix:
      {
        auto echoSend = eb->findParameterByID({ Echo_Send, selParam->getVoiceGroup() });
        return { true, echoSend->getControlPositionValue() > 0 && selParam->getControlPositionValue() > 0 };
      }
      case Reverb_Mix:
      {
        auto reverbSend = eb->findParameterByID({ Reverb_Send, selParam->getVoiceGroup() });
        return { true, reverbSend->getControlPositionValue() > 0 && selParam->getControlPositionValue() > 0 };
      }
      default:
        return { false, false };
    }
  }
  return { false, false };
}

PanelUnitSoundMode::PanelUnitSoundMode()
{
}

void PanelUnitSoundMode::setup()
{
  PanelUnitPresetMode::setup();

  removeButtonConnection(Buttons::BUTTON_SOUND);

  setupButtonConnection(Buttons::BUTTON_SOUND,
                        [&](Buttons button, ButtonModifiers modifiers, bool state)
                        {
                          if(state)
                          {
                            auto& settings = *Application::get().getSettings();
                            SettingsUseCases useCases(settings);
                            auto& famSetting = *settings.getSetting<FocusAndModeSetting>();

                            auto focusAndMode = famSetting.getState();
                            if(focusAndMode.focus == UIFocus::Sound)
                              if(focusAndMode.mode == UIMode::Edit || focusAndMode.detail == UIDetail::Voices)
                                useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                              else
                                useCases.setFocusAndMode(famSetting.getOldState());
                            else
                              useCases.setFocusAndMode(FocusAndMode { UIFocus::Sound });
                          }

                          return true;
                        });
}
