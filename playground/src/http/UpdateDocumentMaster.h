#pragma once

#include "UpdateDocumentContributor.h"

class UpdateDocumentMaster : public UpdateDocumentContributor
{
 public:
  UpdateDocumentMaster();
  virtual ~UpdateDocumentMaster();

  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;
};
