#include <Application.h>
#include "InitSound.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>

InitSound::InitSound(const Rect &rect)
    : AnimatedGenericItem("Init Sound", rect,
                          [] {
                            auto pm = Application::get().getPresetManager();
                            auto scope = pm->getUndoScope().startTransaction("Init Sound");
                            pm->getEditBuffer()->undoableInitSound(scope->getTransaction());
                          },
                          []() {
                            auto hwui = Application::get().getHWUI();
                            hwui->setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                          })
{
}
