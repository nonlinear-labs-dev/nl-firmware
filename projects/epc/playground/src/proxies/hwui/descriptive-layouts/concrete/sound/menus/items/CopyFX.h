#pragma once
#include "nltools/Types.h"
#include "use-cases/SettingsUseCases.h"
#include "proxies/hwui/descriptive-layouts/concrete/menu/menu-items/AnimatedGenericItem.h"
#include <use-cases/EditBufferUseCases.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

template <VoiceGroup from, VoiceGroup to> class CopyFX : public AnimatedGenericItem
{
 public:
  explicit CopyFX(Rect fullWidth);
};

template <VoiceGroup from, VoiceGroup to>
CopyFX<from, to>::CopyFX(Rect fullWidth)
    : AnimatedGenericItem(nltools::string::concat("Copy FX ", toString(from), " to ", toString(to)), fullWidth,
                          OneShotTypes::StartCB { []() {
                            auto eb = Application::get().getPresetManager()->getEditBuffer();
                            EditBufferUseCases ebUseCases(*eb);
                            ebUseCases.copyFX(from, to);
                          } },
                          OneShotTypes::FinishCB { []() {
                            auto eb = Application::get().getPresetManager()->getEditBuffer();
                            SettingsUseCases sUseCases(eb->getSettings());
                            sUseCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                          } })
{
}
