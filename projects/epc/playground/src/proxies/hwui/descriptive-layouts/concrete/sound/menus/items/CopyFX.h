#pragma once
#include "proxies/hwui/descriptive-layouts/concrete/menu/menu-items/GenericItem.h"
#include "nltools/Types.h"
#include "use-cases/SettingsUseCases.h"
#include <use-cases/EditBufferUseCases.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>


template<VoiceGroup from, VoiceGroup to>
class CopyFX : public GenericItem
{
 public:
  explicit CopyFX(Rect fullWidth);
};

template <VoiceGroup from, VoiceGroup to> CopyFX<from, to>::CopyFX(Rect fullWidth) : GenericItem(nltools::string::concat("Copy FX ", toString(from), " to ", toString(to)), fullWidth, [](){
      auto eb = Application::get().getPresetManager()->getEditBuffer();
      EditBufferUseCases ebUseCases(*eb);
      ebUseCases.copyFX(from, to);
      SettingsUseCases sUseCases(eb->getSettings());
      sUseCases.setFocusAndMode({UIFocus::Sound, UIMode::Select, UIDetail::Init});
    })
{
}
