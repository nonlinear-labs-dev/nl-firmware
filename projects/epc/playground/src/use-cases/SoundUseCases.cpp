#include "SoundUseCases.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <libundo/undo/Scope.h>

SoundUseCases::SoundUseCases(EditBuffer* eb, PresetManager* pm)
    : m_editBuffer { eb }
    , m_presetManager { pm }
{
}

void SoundUseCases::storeInitSound()
{
  auto scope = m_presetManager->getUndoScope().startTransaction("Store Init Sound");
  m_presetManager->storeInitSound(scope->getTransaction());
}

void SoundUseCases::resetInitSound()
{
  auto scope = m_presetManager->getUndoScope().startTransaction("Reset Init Sound");
  m_presetManager->resetInitSound(scope->getTransaction());
}

void SoundUseCases::renamePart(VoiceGroup part, const Glib::ustring& name)
{
  auto scope = m_presetManager->getUndoScope().startTransaction("Rename Part");
  m_editBuffer->setVoiceGroupName(scope->getTransaction(), name, part);
}
