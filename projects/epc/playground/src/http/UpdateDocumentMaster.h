#pragma once

#include "UpdateDocumentContributor.h"

class UpdateDocumentMaster : public UpdateDocumentContributor
{
 public:
  UpdateDocumentMaster();
  ~UpdateDocumentMaster() override;

  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;
};
