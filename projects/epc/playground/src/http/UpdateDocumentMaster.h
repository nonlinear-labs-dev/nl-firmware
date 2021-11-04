#pragma once

#include "UpdateDocumentContributor.h"
#include <nltools/include/nltools/messaging/API.h>

class SyncMaster;

class UpdateDocumentMaster : public UpdateDocumentContributor
{
 public:
  UpdateDocumentMaster();
  ~UpdateDocumentMaster() override;

  void enableSync(nltools::msg::API::Backend apiImpl);
  [[nodiscard]] SyncMaster &getSyncMaster() const;

  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;
  uint64_t getAndResetFlags();

 private:
  uint64_t m_flags = 0;
  std::unique_ptr<SyncMaster> m_sync;
};
