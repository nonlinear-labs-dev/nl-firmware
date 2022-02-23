#include "EditBufferStorePreparation.h"
#include <presets/EditBuffer.h>
#include <libundo/undo/Scope.h>

EditBufferStorePreparation::EditBufferStorePreparation(EditBuffer& eb) : m_eb{eb}
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();
  m_oldPositions = m_eb.setHWSourcesToTargetPostion(transaction);
}

EditBufferStorePreparation::~EditBufferStorePreparation()
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();
  m_eb.setHWSourcesToOldPositions(transaction, m_oldPositions);
}
