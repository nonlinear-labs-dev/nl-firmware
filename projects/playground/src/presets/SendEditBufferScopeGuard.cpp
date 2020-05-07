#include "SendEditBufferScopeGuard.h"
#include "presets/EditBuffer.h"

SendEditBufferScopeGuard::SendEditBufferScopeGuard(EditBuffer* eb, UNDO::Transaction* transaction)
    : m_editBuffer { eb }
    , m_transaction { transaction }
{
  m_transaction->addUndoCommand([this](auto s) { m_editBuffer->sendToAudioEngine(); });
}

SendEditBufferScopeGuard::~SendEditBufferScopeGuard()
{
  m_transaction->addDoRedoCommand([this](auto) { m_editBuffer->sendToAudioEngine(); });
}
