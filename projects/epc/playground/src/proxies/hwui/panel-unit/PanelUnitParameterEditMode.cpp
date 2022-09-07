#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <groups/MacroControlMappingGroup.h>
#include <groups/ScaleGroup.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/ScaleParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <device-settings/SignalFlowIndicationSetting.h>
#include <proxies/hwui/TwoStateLED.h>
#include "PanelUnitParameterEditMode.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include <device-settings/LayoutMode.h>
#include <proxies/hwui/descriptive-layouts/GenericLayout.h>
#include <sigc++/sigc++.h>
#include <glibmm/main.h>
#include <groups/MacroControlsGroup.h>
#include <parameters/MacroControlSmoothingParameter.h>

class ParameterInfoLayout;

class ParameterLayout2;

const constexpr auto CombFilterAB = 113;
const constexpr auto CombFilterPM = 133;
const constexpr auto CombFilterPMAB = 135;
const constexpr auto effectsID = 160;
const constexpr auto SVFilterAB = 136;
const constexpr auto SVFilterFM = 153;
const constexpr auto SVFilterFMAB = 155;

PanelUnitParameterEditMode::PanelUnitParameterEditMode()
{
  Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->onChange(
      sigc::hide(sigc::mem_fun(this, &PanelUnitParameterEditMode::bruteForceUpdateLeds)));

  Application::get().getPresetManager()->getEditBuffer()->onPresetLoaded(
      sigc::mem_fun(this, &PanelUnitParameterEditMode::bruteForceUpdateLeds));
}

PanelUnitParameterEditMode::~PanelUnitParameterEditMode()
{
}

ButtonParameterMapping &PanelUnitParameterEditMode::getMappings()
{
  return m_mappings;
}

void PanelUnitParameterEditMode::setupFocusAndMode(FocusAndMode focusAndMode)
{
  super::setupFocusAndMode(focusAndMode);
  bruteForceUpdateLeds();
}

void PanelUnitParameterEditMode::setup()
{
  m_mappings.forEachButton([=](Buttons buttonID, std::list<int> parameterIDs) {
    std::vector<int> para { parameterIDs.begin(), parameterIDs.end() };

    if(buttonID != Buttons::BUTTON_75 && buttonID != Buttons::BUTTON_79 && buttonID != Buttons::BUTTON_83
       && buttonID != Buttons::BUTTON_87 && buttonID != Buttons::BUTTON_91 && buttonID != Buttons::BUTTON_95)
      setupButtonConnection(buttonID, createParameterSelectAction(para));
  });

  using namespace C15::PID;

  setupButtonConnection(Buttons::BUTTON_75,
                        [this](auto &&, auto &&, auto && PH3) { return handleMacroControlButton(std::forward<decltype(PH3)>(PH3), MC_A); });
  FOR_TESTS(assignedAudioIDs.insert(MC_A));

  setupButtonConnection(Buttons::BUTTON_79,
                        [this](auto &&, auto &&, auto && PH3) { return handleMacroControlButton(std::forward<decltype(PH3)>(PH3), MC_B); });
  FOR_TESTS(assignedAudioIDs.insert(MC_B));

  setupButtonConnection(Buttons::BUTTON_83,
                        [this](auto &&, auto &&, auto && PH3) { return handleMacroControlButton(std::forward<decltype(PH3)>(PH3), MC_C); });
  FOR_TESTS(assignedAudioIDs.insert(MC_C));

  setupButtonConnection(Buttons::BUTTON_87,
                        [this](auto &&, auto &&, auto && PH3) { return handleMacroControlButton(std::forward<decltype(PH3)>(PH3), MC_D); });
  FOR_TESTS(assignedAudioIDs.insert(MC_D));

  setupButtonConnection(Buttons::BUTTON_91,
                        [this](auto &&, auto &&, auto && PH3) { return handleMacroControlButton(std::forward<decltype(PH3)>(PH3), MC_E); });
  FOR_TESTS(assignedAudioIDs.insert(MC_E));

  setupButtonConnection(Buttons::BUTTON_95,
                        [this](auto &&, auto &&, auto && PH3) { return handleMacroControlButton(std::forward<decltype(PH3)>(PH3), MC_F); });
  FOR_TESTS(assignedAudioIDs.insert(MC_F));

  setupButtonConnection(Buttons::BUTTON_UNDO, [&](Buttons button, ButtonModifiers modifiers, bool state) {
    m_undoStateMachine.traverse(state ? UNDO_PRESSED : UNDO_RELEASED);
    return false;
  });

  setupButtonConnection(Buttons::BUTTON_REDO, [&](Buttons button, ButtonModifiers modifiers, bool state) {
    m_undoStateMachine.traverse(state ? REDO_PRESSED : REDO_RELEASED);
    return false;
  });

  setupButtonConnection(Buttons::BUTTON_SOUND, [&](Buttons button, ButtonModifiers modifiers, bool state) {
    if(state)
    {
      SettingsUseCases useCases(*Application::get().getSettings());
      auto focusAndMode = Application::get().getSettings()->getSetting<FocusAndModeSetting>()->getState();
      if(focusAndMode.focus == UIFocus::Sound)
        if(focusAndMode.mode == UIMode::Edit)
          useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
        else
          useCases.setFocusAndMode({ UIFocus::Parameters, UIMode::Select });
      else
        useCases.setFocusAndMode(FocusAndMode { UIFocus::Sound });
    }

    return true;
  });

  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::mem_fun(this, &PanelUnitParameterEditMode::onParamSelectionChanged),
      Application::get().getVGManager()->getCurrentVoiceGroup());

  Application::get().getMainContext()->signal_idle().connect_once([=]() {
    auto hwui = Application::get().getHWUI();
    auto &panelUnit = hwui->getPanelUnit();
    auto &famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();

    if(panelUnit.getUsageMode().get() == this)
    {
      panelUnit.getEditPanel().getBoled().setupFocusAndMode(famSetting.getState());
      bruteForceUpdateLeds();
    }
  });
}

bool PanelUnitParameterEditMode::handleMacroControlButton(bool state, int mcParamId)
{
  auto &mcStateMachine = getMacroControlAssignmentStateMachine();
  mcStateMachine.setCurrentMCParameter(mcParamId);

  auto &famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();

  bool isAlreadySelected = Application::get()
                               .getPresetManager()
                               ->getEditBuffer()
                               ->getSelected(Application::get().getVGManager()->getCurrentVoiceGroup())
                               ->getID()
                               .getNumber()
          == mcParamId
      && famSetting.getState().focus == UIFocus::Parameters;

  if(state)
    if(mcStateMachine.traverse(isAlreadySelected ? MacroControlAssignmentEvents::MCPressedWhileSelected
                                                 : MacroControlAssignmentEvents::MCPressedWhileUnselected))
      return true;

  if(!state)
    if(mcStateMachine.traverse(MacroControlAssignmentEvents::MCReleased))
      return true;

  return true;
}

void PanelUnitParameterEditMode::onParamSelectionChanged(Parameter *oldParam, Parameter *newParam)
{
  if(auto mc = dynamic_cast<MacroControlParameter *>(oldParam))
  {
    if(auto ph = dynamic_cast<PhysicalControlParameter *>(newParam))
    {
      if(auto mcm = dynamic_cast<MacroControlMappingGroup *>(
             Application::get().getPresetManager()->getEditBuffer()->getParameterGroupByID(
                 { "MCM", VoiceGroup::Global })))
      {
        if(auto router = mcm->getModulationRoutingParameterFor(ph, mc))
        {
          ph->setUiSelectedModulationRouter(router->getID());
        }
      }
    }
  }

  bruteForceUpdateLeds();
}

Buttons PanelUnitParameterEditMode::findButtonForParameter(Parameter *param) const
{
  int parameterID = param->getID().getNumber();

  if(dynamic_cast<ScaleParameter *>(param))
    parameterID = ScaleGroup::getScaleBaseParameterNumber();

  return m_mappings.findButton(parameterID);
}

std::list<int> PanelUnitParameterEditMode::getButtonAssignments(Buttons button) const
{
  return m_mappings.findParameters(button);
}

template <typename OutContainer, typename InContainer>
OutContainer cleanParameterIDSForType(const InContainer &ids, SoundType type);

std::list<int> PanelUnitParameterEditMode::getButtonAssignments(Buttons button, SoundType type) const
{
  auto raw = getButtonAssignments(button);
  std::vector<int> ass;
  std::copy(raw.begin(), raw.end(), std::back_inserter(ass));

  auto cl = cleanParameterIDSForType<std::vector<int>>(ass, type);

  std::list<int> ret;
  std::copy(cl.begin(), cl.end(), std::back_inserter(ret));
  return ret;
}

UsageMode::tAction PanelUnitParameterEditMode::createParameterSelectAction(const std::vector<gint32>& toggleAudioIDs)
{
  return std::bind(&PanelUnitParameterEditMode::toggleParameterSelection, this, toggleAudioIDs, std::placeholders::_3);
}

template <typename OutContainer, typename InContainer>
OutContainer cleanParameterIDSForType(const InContainer &ids, SoundType type)
{
  OutContainer ret;

  switch(type)
  {
    case SoundType::Single:
      std::copy_if(ids.begin(), ids.end(), std::back_inserter(ret),
                   [](int id) { return id != C15::PID::FB_Mix_Comb_Src && id != C15::PID::FB_Mix_SVF_Src && id != C15::PID::FB_Mix_FX_Src && id != C15::PID::Out_Mix_To_FX; });
      break;
    case SoundType::Split:
      std::copy_if(ids.begin(), ids.end(), std::back_inserter(ret),
                   [](int id) { return id != C15::PID::FB_Mix_Comb_Src && id != C15::PID::FB_Mix_SVF_Src; });
      break;
    case SoundType::Layer:
    case SoundType::Invalid:
      return ids;
  }
  return ret;
}

bool PanelUnitParameterEditMode::toggleParameterSelection(const std::vector<gint32>& ids, bool state)
{
  if(ids.empty())
    return true;

  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  auto cleanedParameterIdForType = cleanParameterIDSForType<std::vector<int>>(ids, editBuffer->getType());

  if(cleanedParameterIdForType.empty())
    return true;

  auto voiceGroup = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto firstParameterInList = editBuffer->findParameterByID({ cleanedParameterIdForType.front(), voiceGroup });

  auto &mcStateMachine = getMacroControlAssignmentStateMachine();

  if(auto modParam = dynamic_cast<ModulateableParameter *>(firstParameterInList))
  {
    mcStateMachine.setCurrentModulateableParameter({ cleanedParameterIdForType.front(), voiceGroup });

    if(mcStateMachine.traverse(state ? MacroControlAssignmentEvents::ModParamPressed
                                     : MacroControlAssignmentEvents::ModParamReleased))
      return true;
  }

  if(mcStateMachine.getState() != MacroControlAssignmentStates::Initial)
    return true;

  if(state)
  {
    Parameter *selParam = editBuffer->getSelected(Application::get().getVGManager()->getCurrentVoiceGroup());

    if(tryParameterToggleOnMacroControl(cleanedParameterIdForType, selParam))
      return true;

    if(isShowingParameterScreen())
    {
      auto selParamID = selParam->getID();
      auto pos = find(cleanedParameterIdForType.begin(), cleanedParameterIdForType.end(), selParamID.getNumber());

      if(pos != cleanedParameterIdForType.end())
        if(switchToNormalModeInCurrentParameterLayout())
          return true;

      for(auto it = cleanedParameterIdForType.begin(); it != cleanedParameterIdForType.end(); ++it)
      {
        if(*it == selParamID.getNumber())
        {
          auto next = std::next(it);

          if(next == cleanedParameterIdForType.end())
            next = cleanedParameterIdForType.begin();

          if(auto newP = editBuffer->findParameterByID({*next, selParamID.getVoiceGroup()}))
          {
            if(!newP->isDisabled())
              setParameterSelection({ *next, selParamID.getVoiceGroup() }, state);
          }
          return true;
        }
      }

      if(ParameterId::isGlobal(cleanedParameterIdForType.front()))
        setParameterSelection({ cleanedParameterIdForType.front(), VoiceGroup::Global }, state);
      else
        setParameterSelection(
            { cleanedParameterIdForType.front(), Application::get().getVGManager()->getCurrentVoiceGroup() }, state);
    }
    else
    {
      if(Application::get().getHWUI()->getButtonModifiers()[SHIFT])
      {
        for(auto paramId : cleanedParameterIdForType)
        {
          auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
          if(ParameterId::isGlobal(paramId))
            vg = VoiceGroup::Global;

          auto param = editBuffer->findParameterByID({ paramId, vg });
          if(param->isChangedFromLoaded())
          {
            setParameterSelection({ paramId, vg }, state);
            return true;
          }
        }
      }

      auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
      setParameterSelection({ cleanedParameterIdForType.front(), vg }, state);
    }
  }

  return true;
}

bool PanelUnitParameterEditMode::switchToNormalModeInCurrentParameterLayout()
{
  auto layout = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().getLayout();

  if(auto p = std::dynamic_pointer_cast<ModulateableParameterLayout2>(layout))
  {
    return p->switchToNormalMode();
  }
  return false;
}

bool PanelUnitParameterEditMode::tryParameterToggleOnMacroControl(const std::vector<gint32>& ids, Parameter *selParam)
{
  if(auto mc = dynamic_cast<MacroControlParameter *>(selParam))
  {
    std::list<ParameterId> a;
    for(auto x : ids)
    {
      auto mcVg = mc->getVoiceGroup();
      a.emplace_back( x, mcVg );
    }

    if(mc->isSourceOfTargetIn(a))
    {
      for(const auto& targetId : a)
      {
        if(mc->isSourceOf(targetId))
        {
          setParameterSelection(targetId, true);
          return true;
        }
      }
    }
  }

  return false;
}

bool PanelUnitParameterEditMode::setParameterSelection(const ParameterId &audioID, bool state)
{
  if(getMacroControlAssignmentStateMachine().getState() != MacroControlAssignmentStates::Initial)
    return true;

  if(state)
  {
    auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
    EditBufferUseCases ebUseCases { *editBuffer };
    ebUseCases.selectParameter(audioID, true);
  }

  return true;
}

bool PanelUnitParameterEditMode::isShowingParameterScreen() const
{
  auto settingValue = Application::get().getSettings()->getSetting<LayoutMode>()->get();
  auto currentLayout = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().getLayout().get();

  if(settingValue == LayoutVersionMode::Old)
  {
    if(dynamic_cast<ParameterLayout2 *>(currentLayout))
      return true;

    if(dynamic_cast<ParameterInfoLayout *>(currentLayout))
      return true;
  }
  else if(settingValue == LayoutVersionMode::New || settingValue == LayoutVersionMode::Mixed)
  {
    if(auto genericLayout = dynamic_cast<DescriptiveLayouts::GenericLayout *>(currentLayout))
    {
      auto &prototype = genericLayout->getPrototype();
      if(prototype.getDesiredFocusAndMode().focus == UIFocus::Parameters)
      {
        return prototype.getDesiredFocusAndMode().mode == UIMode::Select;
      }
    }
    else
    {
      if(dynamic_cast<ParameterLayout2 *>(currentLayout))
        return true;

      if(dynamic_cast<ParameterInfoLayout *>(currentLayout))
        return true;
    }
  }
  return false;
}

void PanelUnitParameterEditMode::bruteForceUpdateLeds()
{
  m_connectionToMacroControl.disconnect();

  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  if(Parameter *selParam = editBuffer->getSelected(Application::get().getVGManager()->getCurrentVoiceGroup()))
  {
    auto selParamID = selParam->getID();

    tLedStates states;
    states.fill(false);

    collectLedStates(states, selParamID);

    if(auto p = dynamic_cast<PhysicalControlParameter *>(selParam))
    {
      auto selModRouter = p->getUiSelectedModulationRouter();

      if(auto router = dynamic_cast<ModulationRoutingParameter *>(editBuffer->findParameterByID(selModRouter)))
      {
        collectLedStates(states, router->getTargetParameter()->getID());
      }
    }

    if(auto router = dynamic_cast<ModulationRoutingParameter *>(selParam))
    {
      if(auto tgt = router->getTargetParameter())
        collectLedStates(states, tgt->getID());
    }

    setLedStates(states);

    if(selParam->getParentGroup()->getID().getName() == "MCs" || MacroControlsGroup::isMacroTime(selParam->getID()))
    {
      letMacroControlTargetsBlink();
    }

    if(Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->get())
      letTargetsBlink(selParam);
  }
}

void PanelUnitParameterEditMode::letTargetsBlink(Parameter *selParam)
{
  auto group = selParam->getParentGroup();
  auto groupName = group->getID().getName();

  using namespace C15::PID;

  if(groupName == "Env A")
  {
    letOtherTargetsBlink({ Osc_A_PM_Self_Env_A, Shp_A_Drive_Env_A, Osc_B_PM_A_Env_A });
  }
  else if(groupName == "Env B")
  {
    letOtherTargetsBlink({ Osc_A_PM_B_Env_B, Osc_B_PM_Self_Env_B, Shp_B_Drive_Env_B });
  }
  else if(groupName == "Env C")
  {
    letOtherTargetsBlink({ Osc_A_Pitch_Env_C, Osc_A_Fluct_Env_C, Osc_A_PM_FB_Env_C, Shp_A_FB_Env_C, Osc_B_Pitch_Env_C,
                           Osc_B_Fluct_Env_C, Osc_B_PM_FB_Env_C, Shp_B_FB_Env_C, Comb_Flt_Pitch_Env_C, Comb_Flt_AP_Env_C,
                           Comb_Flt_LP_Env_C, SV_Flt_Cut_Env_C, SV_Flt_Res_Env_C });
  }
  else if(groupName == "Osc A" || groupName == "Sh A")
  {
    letOscAShaperABlink({ Osc_B_PM_A, Shp_B_Ring_Mod, Comb_Flt_In_A_B, Comb_Flt_PM, SV_Flt_In_A_B, SV_Flt_FM, Out_Mix_A_Lvl });
  }
  else if(groupName == "Osc B" || groupName == "Sh B")
  {
    letOscBShaperBBlink({ Osc_A_PM_B, Shp_A_Ring_Mod, Comb_Flt_In_A_B, Comb_Flt_PM, SV_Flt_In_A_B, SV_Flt_FM, Out_Mix_B_Lvl });
  }
  else if(groupName == "Comb")
  {
    letOtherTargetsBlink({ SV_Flt_Comb_Mix, Out_Mix_Comb_Lvl, FB_Mix_Comb });
  }
  else if(groupName == "FB")
  {
    letOtherTargetsBlink({ Osc_A_PM_FB, Shp_A_FB_Mix, Osc_B_PM_FB, Shp_B_FB_Mix });
  }
  else if(groupName == "SVF")
  {
    letOtherTargetsBlink({ Out_Mix_SVF_Lvl, FB_Mix_SVF });
  }
  else if(groupName == "Gap Filt" || groupName == "Cab" || groupName == "Echo" || groupName == "Reverb"
          || groupName == "Flang")
  {
    letOtherTargetsBlink({ FB_Mix_FX });

    if(groupName == "Reverb")
    {
      letReverbBlink({ FB_Mix_Rvb });
    }
  }
}

bool isScaleParameter(const ParameterId &paramID)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->findAndCastParameterByID<ScaleParameter>(paramID) != nullptr;
}

void PanelUnitParameterEditMode::collectLedStates(tLedStates &states, ParameterId selectedParameterID)
{
  if(isScaleParameter(selectedParameterID))
  {
    selectedParameterID = ParameterId(ScaleGroup::getScaleBaseParameterNumber(), selectedParameterID.getVoiceGroup());
  }

  if(MacroControlsGroup::isMacroTime(selectedParameterID))
  {
    selectedParameterID = MacroControlsGroup::smoothingIdToMCId(selectedParameterID);
  }

  auto button = m_mappings.findButton(selectedParameterID.getNumber());

  if(button != Buttons::INVALID)
    states[static_cast<size_t>(button)] = true;
}

const BOLED &PanelUnitParameterEditMode::getBoled() const
{
  return Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
}

BOLED &PanelUnitParameterEditMode::getBoled()
{
  return Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
}

void PanelUnitParameterEditMode::setLedStates(const tLedStates &states)
{
  auto &panelUnit = Application::get().getHWUI()->getPanelUnit();

  for(int i = 0; i < NUM_LEDS; i++)
    panelUnit.getLED((Buttons) i)->setState(states[i] ? TwoStateLED::ON : TwoStateLED::OFF);
}

void PanelUnitParameterEditMode::letMacroControlTargetsBlink()
{
  auto &panelUnit = Application::get().getHWUI()->getPanelUnit();
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  Parameter *selParam = editBuffer->getSelected(Application::get().getVGManager()->getCurrentVoiceGroup());
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();

  if(auto modTime = dynamic_cast<MacroControlSmoothingParameter *>(selParam))
  {
    selParam = editBuffer->findParameterByID(modTime->getMC());
  }

  if(auto mc = dynamic_cast<MacroControlParameter *>(selParam))
  {
    for(auto t : mc->getTargets())
    {
      if(t->getID().getVoiceGroup() == currentVG)
      {
        auto buttonID = m_mappings.findButton(t->getID().getNumber());
        if(buttonID != Buttons::INVALID)
          panelUnit.getLED(buttonID)->setState(TwoStateLED::BLINK);
      }
    }

    m_connectionToMacroControl
        = mc->onTargetListChanged(mem_fun(this, &PanelUnitParameterEditMode::bruteForceUpdateLeds));
  }
}

void PanelUnitParameterEditMode::letOtherTargetsBlink(const std::vector<int> &targets)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto hwui = Application::get().getHWUI();
  auto &panelUnit = hwui->getPanelUnit();

  for(auto targetID : targets)
  {
    auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
    const auto currentParam = editBuffer->findParameterByID({ targetID, vg });

    if(isSignalFlowingThrough(currentParam))
    {
      auto state = editBuffer->getSelected(vg) == currentParam ? TwoStateLED::ON
                                                                                         : TwoStateLED::BLINK;
      panelUnit.getLED(m_mappings.findButton(targetID))->setState(state);
    }
  }
}

bool PanelUnitParameterEditMode::isSignalFlowingThrough(const Parameter *p) const
{
  return std::abs(p->getControlPositionValue()) > std::numeric_limits<tControlPositionValue>::epsilon();
}

void PanelUnitParameterEditMode::letOscAShaperABlink(const std::vector<int> &targets)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto &panelUnit = Application::get().getHWUI()->getPanelUnit();

  const auto stateVariableFilterFMAB = editBuffer->findParameterByID({ SVFilterFMAB, vg });
  const auto combFilterPMAB = editBuffer->findParameterByID({ CombFilterPMAB, vg });
  const auto SVCombMix = editBuffer->findParameterByID({ C15::PID::SV_Flt_Comb_Mix, vg });
  const auto combMax = SVCombMix->getValue().getUpperBorder();
  const auto combMin = SVCombMix->getValue().getLowerBorder();

  for(auto targetID : targets)
  {
    const auto currentParam = editBuffer->findParameterByID({ targetID, vg });
    switch(targetID)
    {
      case SVFilterAB:
        if(SVCombMix->getControlPositionValue() != combMin && SVCombMix->getControlPositionValue() != combMax)
          if(currentParam->getControlPositionValue() < 1)
            panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      case CombFilterAB:
        if(currentParam->getControlPositionValue() < 1)
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      case CombFilterPM:
        if(isSignalFlowingThrough(currentParam) && combFilterPMAB->getControlPositionValue() < 1)
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      case SVFilterFM:
        if(isSignalFlowingThrough(currentParam) && stateVariableFilterFMAB->getControlPositionValue() < 1)
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      default:
        if(isSignalFlowingThrough(currentParam))
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
    }
  }
}

void PanelUnitParameterEditMode::letOscBShaperBBlink(const std::vector<int> &targets)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto &panelUnit = Application::get().getHWUI()->getPanelUnit();

  const auto combFilterPMAB = editBuffer->findParameterByID({ CombFilterPMAB, vg });
  const auto stateVariableFilterFMAB = editBuffer->findParameterByID({ SVFilterFMAB, vg });
  const auto SVCombMix = editBuffer->findParameterByID({ C15::PID::SV_Flt_Comb_Mix, vg });
  const auto combMax = SVCombMix->getValue().getUpperBorder();
  const auto combMin = SVCombMix->getValue().getLowerBorder();

  for(auto targetID : targets)
  {
    auto currentParam = editBuffer->findParameterByID({ targetID, vg });
    switch(targetID)
    {
      case SVFilterAB:
        if(SVCombMix->getControlPositionValue() != combMin && SVCombMix->getControlPositionValue() != combMax)
          if(currentParam->getControlPositionValue() > 0)
            panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      case CombFilterAB:
        if(currentParam->getControlPositionValue() > 0)
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      case CombFilterPM:
        if(isSignalFlowingThrough(currentParam) && combFilterPMAB->getControlPositionValue() > 0)
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      case SVFilterFM:
        if(isSignalFlowingThrough(currentParam) && stateVariableFilterFMAB->getControlPositionValue() > 0)
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
      default:
        if(isSignalFlowingThrough(currentParam))
          panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
        break;
    }
  }
}

void PanelUnitParameterEditMode::letReverbBlink(const std::vector<int> &targets)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto &panelUnit = Application::get().getHWUI()->getPanelUnit();

  for(auto targetID : targets)
  {
    const auto currentParam = editBuffer->findParameterByID({ targetID, vg });
    const auto effectParam = editBuffer->findParameterByID({ effectsID, vg });

    if(isSignalFlowingThrough(currentParam) && isSignalFlowingThrough(effectParam))
      panelUnit.getLED(m_mappings.findButton(targetID))->setState(TwoStateLED::BLINK);
  }
}

MacroControlAssignmentStateMachine &PanelUnitParameterEditMode::getMacroControlAssignmentStateMachine()
{
  return Application::get().getHWUI()->getPanelUnit().getMacroControlAssignmentStateMachine();
}
