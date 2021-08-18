#pragma once
#include <http/UpdateDocumentMaster.h>

class SyncMasterMockRoot : public UpdateDocumentMaster
{
 private:
  SyncMasterMockRoot();

 public:
  static SyncMasterMockRoot &get();
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
};
