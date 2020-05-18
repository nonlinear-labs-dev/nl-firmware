#pragma once

#include <libundo/undo/Transaction.h>
class EditBuffer;
class SendEditBufferScopeGuard
{
 public:
  SendEditBufferScopeGuard(EditBuffer* eb, UNDO::Transaction* transaction);
  ~SendEditBufferScopeGuard();

 private:
  UNDO::Transaction* m_transaction;
  EditBuffer* m_editBuffer;
};

class GenericEditBufferScopeGuard
{
 public:
  template <typename tCB>
  GenericEditBufferScopeGuard(const tCB& enter, const tCB& exit)
      : m_end { exit }
  {
    enter();
  }

  ~GenericEditBufferScopeGuard()
  {
    m_end();
  }

 private:
  std::function<void(void)> m_end;
};