#include "UndoScope.h"
#include "xml/Writer.h"
#include <presets/EditBufferSnapshotMaker.h>

void UndoScope::onAddTransaction (UNDO::Scope::tTransactionPtr transaction)
{
  EditBufferSnapshotMaker::get().addSnapshotIfRequired(transaction);
  UNDO::Scope::onAddTransaction (transaction);
}

UndoScope::tUpdateID UndoScope::onChange ()
{
  auto i = super::onChange ();
  m_sigUndoScopeChanged.send ();
  return i;
}

sigc::connection UndoScope::onUndoScopeChanged (slot<void> cb)
{
  return m_sigUndoScopeChanged.connectAndInit (cb);
}
