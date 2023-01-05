#include <utility>
#include "GlobalEventSinkBroker.h"
#include <parameters/ModulateableParameter.h>
#include <libundo/undo/Scope.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterCarousel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationCarousel.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetList.h>
#include <proxies/hwui/descriptive-layouts/concrete/preset/GenericPresetList.h>
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/HWUI.h"
#include "proxies/hwui/Layout.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/ParameterUseCases.h"
#include "use-cases/MacroControlParameterUseCases.h"
#include "use-cases/ModParameterUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include "use-cases/VoiceGroupUseCases.h"
#include <proxies/hwui/descriptive-layouts/GenericLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableDualVoiceGroupMasterAndSplitPointLayout.h>
#include <proxies/hwui/controls/SwitchVoiceGroupButton.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <parameter_declarations.h>
#include <use-cases/DirectLoadUseCases.h>

namespace DescriptiveLayouts
{

  GlobalEventSinkBroker::GlobalEventSinkBroker()
  {
    auto eb = Application::get().getPresetManager()->getEditBuffer();
    auto hwui = Application::get().getHWUI();
    auto vgManager = Application::get().getVGManager();

    registerEvent(EventSinks::Swallow, []() { return; });

    registerEvent(EventSinks::IncParam,
                  [eb, hwui, vgManager]()
                  {
                    if(auto p = eb->getSelected(vgManager->getCurrentVoiceGroup()))
                    {
                      auto modifiers = hwui->getButtonModifiers();
                      ParameterUseCases useCase(p);
                      useCase.incDec(1, modifiers[ButtonModifier::FINE], modifiers[ButtonModifier::SHIFT]);
                    }
                  });

    registerEvent(EventSinks::DecParam,
                  [eb, hwui, vgManager]()
                  {
                    if(auto p = eb->getSelected(vgManager->getCurrentVoiceGroup()))
                    {
                      auto modifiers = hwui->getButtonModifiers();
                      ParameterUseCases useCase(p);
                      useCase.incDec(-1, modifiers[ButtonModifier::FINE], modifiers[ButtonModifier::SHIFT]);
                    }
                  });

    registerEvent(EventSinks::IncMCSel,
                  [eb, hwui, vgManager]()
                  {
                    if(auto modParam
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      ModParameterUseCases useCase(modParam);
                      useCase.incMCSelection(1);
                    }
                  });

    registerEvent(EventSinks::DecMCSel,
                  [eb, hwui, vgManager]()
                  {
                    if(auto modParam
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      ModParameterUseCases useCase(modParam);
                      useCase.incMCSelection(-1);
                    }
                  });

    registerEvent(EventSinks::IncMCAmt,
                  [eb, hwui, vgManager]()
                  {
                    if(auto modParam
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      ModParameterUseCases useCase(modParam);
                      useCase.incModAmount(1, false);
                    }
                  });

    registerEvent(EventSinks::DecMCAmt,
                  [eb, hwui, vgManager]()
                  {
                    if(auto modParam
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      ModParameterUseCases useCase(modParam);
                      useCase.incModAmount(-1, false);
                    }
                  });

    registerEvent(EventSinks::IncMCPos,
                  [eb, hwui, vgManager]()
                  {
                    if(auto modP
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      if(auto mc = modP->getMacroControl())
                      {
                        auto hwuiModifiers = hwui->getButtonModifiers();
                        auto fine = hwuiModifiers[ButtonModifier::FINE];
                        auto shift = hwuiModifiers[ButtonModifier::SHIFT];
                        MacroControlParameterUseCases useCase(mc);
                        useCase.incDecPosition(1, fine, shift);
                      }
                    }
                  });

    registerEvent(EventSinks::DecMCPos,
                  [eb, hwui, vgManager]()
                  {
                    if(auto modP
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      if(auto mc = modP->getMacroControl())
                      {
                        auto hwuiModifiers = hwui->getButtonModifiers();
                        auto fine = hwuiModifiers[ButtonModifier::FINE];
                        auto shift = hwuiModifiers[ButtonModifier::SHIFT];
                        MacroControlParameterUseCases useCase(mc);
                        useCase.incDecPosition(-1, fine, shift);
                      }
                    }
                  });

    registerEvent(EventSinks::ToggleVoiceGroupWithParameterSelection,
                  [vgManager, eb]()
                  {
                    VoiceGroupUseCases vgUseCases(vgManager, eb);
                    vgUseCases.toggleVoiceGroupSelection();
                  });

    registerEvent(EventSinks::ToggleVoiceGroup,
                  [vgManager, eb]()
                  {
                    VoiceGroupUseCases vgUseCases(vgManager, eb);
                    vgUseCases.toggleVoiceGroupSelectionSilent();
                  });

    /*
       * UIFocus
       */
    registerEvent(EventSinks::SwitchToParameterFocus,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(FocusAndMode { UIFocus::Parameters });
                  });

    registerEvent(EventSinks::SwitchToSoundFocus,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                  });

    registerEvent(EventSinks::SwitchToPresetFocus,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(FocusAndMode { UIFocus::Presets });
                  });

    registerEvent(EventSinks::SwitchToBankFocus,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(FocusAndMode { UIFocus::Banks });
                  });

    registerEvent(EventSinks::SwitchToSetupFocus,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(FocusAndMode { UIFocus::Setup });
                  });

    /*
       * UIMode
       */
    registerEvent(EventSinks::SwitchToEditMode,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(FocusAndMode { UIMode::Edit });
                  });

    registerEvent(EventSinks::SwitchToSelectMode,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(FocusAndMode { UIMode::Select });
                  });

    /*
       * UIDetail
       */
    registerEvent(EventSinks::SwitchToInitDetail,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(UIDetail::Init);
                  });
    registerEvent(EventSinks::SwitchToButtonADetail,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(UIDetail::ButtonA);
                  });
    registerEvent(EventSinks::SwitchToButtonBDetail,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(UIDetail::ButtonB);
                  });
    registerEvent(EventSinks::SwitchToButtonCDetail,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(UIDetail::ButtonC);
                  });
    registerEvent(EventSinks::SwitchToButtonDDetail,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(UIDetail::ButtonD);
                  });

    registerEvent(EventSinks::SwitchToVoicesDetail,
                  []
                  {
                    SettingsUseCases useCases(*Application::get().getSettings());
                    useCases.setFocusAndMode(UIDetail::Voices);
                  });

    registerEvent(EventSinks::ToggleDirectLoad,
                  [hwui]
                  {
                    DirectLoadUseCases useCase(Application::get().getSettings()->getSetting<DirectLoadSetting>());
                    useCase.toggleDirectLoadFromHWUI(hwui);
                  });

    registerEvent(EventSinks::SwitchToMCAmtDetail,
                  [hwui, eb, vgManager]()
                  {
                    if(auto modParam
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      if(modParam->getModulationSource() != MacroControls::NONE)
                      {
                        SettingsUseCases useCases(*Application::get().getSettings());
                        useCases.setFocusAndMode(UIDetail::MCAmount);
                      }
                    }
                  });

    registerEvent(EventSinks::SwitchToMCModRangeDetail,
                  [hwui, eb, vgManager]()
                  {
                    if(auto modParam
                       = dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup())))
                    {
                      if(modParam->getModulationSource() != MacroControls::NONE)
                      {
                        SettingsUseCases useCases(*Application::get().getSettings());
                        useCases.setFocusAndMode(UIDetail::MCModRange);
                      }
                    }
                  });

    registerEvent(EventSinks::SwitchToMCSelectDetail,
                  [hwui, eb, vgManager]()
                  {
                    if(dynamic_cast<ModulateableParameter *>(eb->getSelected(vgManager->getCurrentVoiceGroup()))
                       != nullptr)
                    {
                      SettingsUseCases useCases(*Application::get().getSettings());
                      useCases.setFocusAndMode(UIDetail::MCSelect);
                    }
                  });

    registerEvent(EventSinks::IncModulationCarousel,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto carousel = genericLayout->findControlOfType<ModulationCarousel>())
                      {
                        carousel->onRotary(1, hwui->getButtonModifiers());
                      }
                    }
                  });

    registerEvent(EventSinks::DecModulationCarousel,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto carousel = genericLayout->findControlOfType<ModulationCarousel>())
                      {
                        carousel->onRotary(-1, hwui->getButtonModifiers());
                      }
                    }
                  });

    registerEvent(EventSinks::IncPresetSelectionPresetList,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto presetList = genericLayout->findControlOfType<GenericPresetList>())
                      {
                        presetList->incPresetSelection();
                      }
                    }
                  });

    registerEvent(EventSinks::DecPresetSelectionPresetList,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto presetList = genericLayout->findControlOfType<GenericPresetList>())
                      {
                        presetList->decPresetSelection();
                      }
                    }
                  });

    registerEvent(EventSinks::DecBankSelectionPresetList,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto presetList = genericLayout->findControlOfType<GenericPresetList>())
                      {
                        presetList->decBankSelection();
                      }
                    }
                  });

    registerEvent(EventSinks::IncBankSelectionPresetList,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto presetList = genericLayout->findControlOfType<GenericPresetList>())
                      {
                        presetList->incBankSelection();
                      }
                    }
                  });

    registerEvent(EventSinks::DoPresetListAction,
                  [hwui]()
                  {
                    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
                    if(auto genericLayout = dynamic_cast<GenericLayout *>(layout))
                    {
                      if(auto presetList = genericLayout->findControlOfType<GenericPresetList>())
                      {
                        presetList->action();
                      }
                    }
                  });

    registerEvent(EventSinks::OpenMonoParameterScreen,
                  [eb, vgManager]()
                  {
                    EditBufferUseCases useCase(*eb);
                    auto vg = vgManager->getCurrentVoiceGroup();
                    if(eb->getType() == SoundType::Split)
                      useCase.selectParameter(ParameterId { 364, vg }, true);
                    else
                      useCase.selectParameter(ParameterId { 364, VoiceGroup::I }, true);
                  });

    registerEvent(EventSinks::OpenPartScreen,
                  [eb, vgManager]()
                  {
                    EditBufferUseCases ebUseCases { *eb };
                    ebUseCases.selectParameter({ 358, vgManager->getCurrentVoiceGroup() }, true);
                  });

    registerEvent(EventSinks::OpenMasterParameter,
                  [eb]
                  {
                    EditBufferUseCases ebUseCases { *eb };
                    ebUseCases.selectParameter({ 247, VoiceGroup::Global }, true);
                  });

    registerEvent(EventSinks::OpenFXMixParameter,
                  [eb]
                  {
                    EditBufferUseCases ebUseCases{*eb};
                    ebUseCases.selectParameter({C15::PID::Master_FX_Mix, VoiceGroup::Global}, true);
                  });

    registerEvent(EventSinks::InitSound,
                  [eb]
                  {
                    EditBufferUseCases useCases { *eb };
                    useCases.initSound(Defaults::UserDefault);
                    SettingsUseCases useCase(*Application::get().getSettings());
                    useCase.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                  });

    registerEvent(EventSinks::OpenUnisonParameter,
                  [eb, vgManager]()
                  {
                    auto vg = vgManager->getCurrentVoiceGroup();
                    EditBufferUseCases ebUseCases { *eb };
                    ebUseCases.selectParameter({ 249, vg }, true);
                  });

    registerEvent(EventSinks::IncSplitPoint,
                  [hwui, eb, vgManager]()
                  {
                    EditBufferUseCases ebUseCases(*eb);
                    auto currentVG = vgManager->getCurrentVoiceGroup();
                    if(auto parameterUseCases = ebUseCases.getUseCase({ C15::PID::Split_Split_Point, currentVG }))
                    {
                      parameterUseCases->incDec(1, false, false);
                    }
                  });

    registerEvent(EventSinks::DecSplitPoint,
                  [hwui, eb, vgManager]()
                  {
                    EditBufferUseCases ebUseCases(*eb);
                    auto currentVG = vgManager->getCurrentVoiceGroup();
                    if(auto parameterUseCases = ebUseCases.getUseCase({ C15::PID::Split_Split_Point, currentVG }))
                    {
                      parameterUseCases->incDec(-1, false, false);
                    }
                  });

    registerEvent(EventSinks::LayerMuteInc,
                  [eb]()
                  {
                    auto muteI = eb->findParameterByID({ 395, VoiceGroup::I });
                    auto muteII = eb->findParameterByID({ 395, VoiceGroup::II });
                    const auto vgIMuted = muteI->getControlPositionValue() > 0.5;
                    const auto vgIIMuted = muteII->getControlPositionValue() > 0.5;
                    EditBufferUseCases useCases(*eb);

                    if(!vgIMuted && !vgIIMuted)
                    {
                      useCases.mutePart(VoiceGroup::I);
                    }
                    else if(vgIIMuted)
                    {
                      useCases.unmuteBothPartsWithTransactionNameForPart(VoiceGroup::II);
                    }
                  });

    registerEvent(EventSinks::LayerMuteDec,
                  [eb]()
                  {
                    auto muteI = eb->findParameterByID({ 395, VoiceGroup::I });
                    auto muteII = eb->findParameterByID({ 395, VoiceGroup::II });
                    const auto vgIMuted = muteI->getControlPositionValue() > 0.5;
                    const auto vgIIMuted = muteII->getControlPositionValue() > 0.5;
                    EditBufferUseCases useCases(*eb);

                    if(!vgIMuted && !vgIIMuted)
                    {
                      useCases.mutePart(VoiceGroup::II);
                    }
                    else if(vgIMuted)
                    {
                      useCases.unmuteBothPartsWithTransactionNameForPart(VoiceGroup::I);
                    }
                  });
  }

  void GlobalEventSinkBroker::registerEvent(EventSinks sink, tAction action)
  {
    m_map[sink] = std::move(action);
  }

  void GlobalEventSinkBroker::fire(EventSinks s)
  {
    try
    {
      m_map.at(s)();
    }
    catch(...)
    {
      nltools::Log::error("EventSink not found in current Layout!", toString(s));
    }
  }
}
