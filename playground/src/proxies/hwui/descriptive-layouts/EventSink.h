#pragma once

#include <playground.h>
#include "TemplateEnums.h"
#include <tools/EnumTools.h>
#include <proxies/hwui/buttons.h>

namespace DescriptiveLayouts
{
  ENUM(EventSinks, uint8_t, IncParam, DecParam, SwitchToInitDetail, SwitchToEditMode, SwitchToSelectMode,
       SwitchToSetupFocus, SwitchToParameterFocus, SwitchToBankFocus, SwitchToPresetFocus, SwitchToSoundFocus,
       SwitchToMCSelectDetail, SwitchToButtonADetail, SwitchToButtonBDetail, SwitchToButtonCDetail,
       SwitchToButtonDDetail, SwitchToMCModRangeDetail, SwitchToMCAmtDetail, SelectPresetForVoiceGroup, IncMCSel,
       DecMCSel, IncMCAmt, DecMCAmt, IncMCPos, DecMCPos, IncButtonMenu, DecButtonMenu, FireButtonMenu,
       IncModulationCarousel, DecModulationCarousel, ToggleVoiceGroup, CommitPresetForButtonMenu, ToggleDirectLoad,
       IncPresetSelectionPresetList, DecPresetSelectionPresetList, IncBankSelectionPresetList,
       DecBankSelectionPresetList, DoPresetListAction, OpenUnisonParameter, OpenMasterParameter);

  struct EventSinkMapping
  {
   public:
    EventSinkMapping(Buttons button, EventSinks sink, ButtonEvents event = ButtonEvents::Down, bool rep = false);

    Buttons button;
    ButtonEvents event;
    EventSinks sink;
    bool repeat;
  };

  class EventSinkBroker
  {
   public:
    static EventSinkBroker& get();

    void fire(EventSinks s);

   private:
    using tAction = std::function<void()>;

    EventSinkBroker();
    void registerEvent(EventSinks sink, tAction action);

    std::map<EventSinks, tAction> m_map;
  };
}
