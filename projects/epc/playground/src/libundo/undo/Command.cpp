#include "Command.h"
#include <nltools/Assert.h>

namespace UNDO
{

  Command::Command()
      : m_parent(nullptr)
      , m_state(INITIAL)
  {
  }

  Command::~Command()
  {
  }

  void Command::setParentTransaction(Transaction *parent)
  {
    m_parent = parent;
  }

  bool Command::hasParentTransaction() const
  {
    return m_parent != nullptr;
  }

  Command::State Command::getState() const
  {
    return m_state;
  }

  void Command::doAction() const
  {
    nltools_assertAlways(getState() == INITIAL);
    m_state = DOING;
    implDoAction();
    m_state = DONE;
  }

  void Command::undoAction() const
  {
    nltools_assertAlways(getState() == DONE || getState() == REDONE);
    m_state = UNDOING;
    implUndoAction();
    m_state = UNDONE;
  }

  void Command::redoAction() const
  {
    nltools_assertAlways(getState() == UNDONE);
    m_state = REDOING;
    implRedoAction();
    m_state = REDONE;
  }
} /* namespace UNDO */
