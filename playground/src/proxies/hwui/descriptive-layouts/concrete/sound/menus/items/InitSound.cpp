#include <Application.h>
#include "InitSound.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

InitSound::InitSound(const Rect &rect)
    : AnimatedGenericItem("Init Sound", rect,
                          [] {
                            auto pm = Application::get().getPresetManager();
                            auto scope = pm->getUndoScope().startTransaction("Init Sound");
                            pm->getEditBuffer()->undoableInitSound(scope->getTransaction());
                          },
                          []() {})
{
}
