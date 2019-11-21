#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include "RenameVoiceGroupLayout.h"

inline EditBuffer* getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

RenameVoiceGroupLayout::RenameVoiceGroupLayout(VoiceGroup vg)
    : m_targetGroup{ vg }
{
}

void RenameVoiceGroupLayout::cancel()
{
  Application::get().getHWUI()->setFocusAndMode(FocusAndMode(UIFocus::Sound, UIMode::Select, UIDetail::Init));
}

void RenameVoiceGroupLayout::commit(const Glib::ustring& newName)
{
  auto eb = getEditBuffer();
  auto scope = eb->getParent()->getUndoScope().startTransaction("Rename Part to %s", newName);
  eb->setVoiceGroupName(scope->getTransaction(), newName, m_targetGroup);
  Application::get().getHWUI()->setFocusAndMode(FocusAndMode(UIFocus::Sound, UIMode::Select, UIDetail::Init));
}

Glib::ustring RenameVoiceGroupLayout::getInitialText() const
{
  return getEditBuffer()->getVoiceGroupName(m_targetGroup);
}
