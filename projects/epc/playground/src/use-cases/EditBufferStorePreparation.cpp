#include "EditBufferStorePreparation.h"
#include <presets/EditBuffer.h>
#include <libundo/undo/Scope.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

EditBufferStorePreparation::EditBufferStorePreparation(EditBuffer& eb) : m_eb{eb}
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();

  using namespace C15::PID;
  for(auto id: { Pedal_1, Pedal_2, Pedal_3, Pedal_4, Ribbon_1, Ribbon_2, Ribbon_3, Ribbon_4, Bender, Aftertouch })
  {
    if(auto hw_param = eb.findAndCastParameterByID<PhysicalControlParameter>({id, VoiceGroup::Global}))
    {
      hw_param->resetChangedFromHWUIPosition();
    }
  }

  m_eb.getAudioEngineProxy().freezeParameterMessages();
  m_eb.getAudioEngineProxy().freezePresetMessages();
  m_oldPositions = m_eb.setHWSourcesToDefaultValues(transaction);
}

EditBufferStorePreparation::~EditBufferStorePreparation()
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();
  m_eb.setHWSourcesToOldPositions(transaction, m_oldPositions);
  m_eb.getAudioEngineProxy().thawParameterMessages(false);
  m_eb.getAudioEngineProxy().thawPresetMessages(false);
}
