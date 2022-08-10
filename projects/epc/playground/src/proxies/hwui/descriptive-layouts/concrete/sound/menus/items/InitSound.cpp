#include <Application.h>
#include "InitSound.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <libundo/undo/Scope.h>

InitSound::InitSound(const Rect& rect)
    : AnimatedGenericItem("Init Sound", rect,
                          OneShotTypes::StartCB(
                              []
                              {
                                auto pm = Application::get().getPresetManager();
                                EditBufferUseCases useCase { *pm->getEditBuffer() };
                                useCase.initSound(Defaults::UserDefault);
                              }),
                          OneShotTypes::FinishCB(
                              []()
                              {
                                SettingsUseCases useCases(*Application::get().getSettings());
                                useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                              }))
{
}

InitPart::InitPart(const Rect& r)
    : AnimatedGenericItem("Init Part", r,
                          OneShotTypes::StartCB(
                              []()
                              {
                                auto pm = Application::get().getPresetManager();
                                EditBufferUseCases useCases { *pm->getEditBuffer() };
                                auto currentPart = Application::get().getVGManager()->getCurrentVoiceGroup();
                                useCases.initPart(currentPart, Defaults::UserDefault);
                              }),
                          OneShotTypes::FinishCB(
                              []()
                              {
                                SettingsUseCases useCases(*Application::get().getSettings());
                                useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                              }))
{
}
