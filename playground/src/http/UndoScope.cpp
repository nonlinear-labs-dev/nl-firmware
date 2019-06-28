#include "UndoScope.h"
#include "xml/Writer.h"

UndoScope::tUpdateID UndoScope::onChange(uint64_t flags)
{
  auto i = super::onChange(flags);
  m_sigUndoScopeChanged.send();
  return i;
}

sigc::connection UndoScope::onUndoScopeChanged(slot<void> cb)
{
  return m_sigUndoScopeChanged.connectAndInit(cb);
}
