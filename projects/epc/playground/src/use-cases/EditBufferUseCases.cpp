#include "EditBufferUseCases.h"
#include <presets/EditBuffer.h>
#include <presets/Preset.h>
#include <libundo/undo/Scope.h>

EditBufferUseCases::EditBufferUseCases(EditBuffer* eb)
    : m_editBuffer { eb }
{
}

void EditBufferUseCases::resetCustomScale()
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Reset Custom Scale");
  auto scaleGroup = m_editBuffer->getParameterGroupByID({ "Scale", VoiceGroup::Global });
  scaleGroup->undoableReset(scope->getTransaction(), Initiator::EXPLICIT_USECASE);
}

void EditBufferUseCases::undoableLoadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to)
{
  auto transactionName = preset->buildUndoTransactionTitle("Load Part " + toString(from) + " To Part " + toString(to));
  auto scope = m_editBuffer->getUndoScope().startTransaction(transactionName);
  m_editBuffer->undoableLoadToPart(scope->getTransaction(), preset, from, to);
}

void EditBufferUseCases::undoableLoad(const Preset* preset)
{
  if(m_editBuffer->getUUIDOfLastLoadedPreset() != preset->getUuid())
  {
    auto scope = m_editBuffer->getUndoScope().startTransaction(preset->buildUndoTransactionTitle("Load"));
    m_editBuffer->undoableLoad(scope->getTransaction(), preset, true);
  }
}