#include "EditBufferStorePreparation.h"
#include <presets/EditBuffer.h>
#include <libundo/undo/Scope.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

EditBufferStorePreparation::EditBufferStorePreparation(EditBuffer& eb) : m_eb{eb}
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();
  m_oldPositions = m_eb.setHWSourcesToDefaultValues(transaction);
  eb.getAudioEngineProxy().freezeParameterMessages();
  eb.getAudioEngineProxy().freezePresetMessages();
}

EditBufferStorePreparation::~EditBufferStorePreparation()
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();
  m_eb.setHWSourcesToOldPositions(transaction, m_oldPositions);
  m_eb.getAudioEngineProxy().thawParameterMessages(false);
  m_eb.getAudioEngineProxy().thawPresetMessages(false);
}
