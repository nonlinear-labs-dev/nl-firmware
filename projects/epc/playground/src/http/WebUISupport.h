#pragma once

#include "playground.h"
#include "http/ContentSection.h"
#include "http/RPCActionManager.h"
#include "SectionAndActionManager.h"

class WebUISupport : public SectionAndActionManager
{
 public:
  explicit WebUISupport(UpdateDocumentContributor *master);


 private:
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
};
