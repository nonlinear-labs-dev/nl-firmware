#pragma once
#include <http/UpdateDocumentMaster.h>
#include <libundo/undo/Scope.h>

class SyncMasterMockRoot : public UpdateDocumentMaster
{
 private:
  SyncMasterMockRoot();

 public:
  static SyncMasterMockRoot &get();
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  UNDO::Scope &getUndoScope() override;

 private:
  UNDO::Scope m_scope;
};
