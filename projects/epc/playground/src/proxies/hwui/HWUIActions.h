#pragma once

#include <http/RPCActionManager.h>
#include <presets/EditBuffer.h>
#include <http/ContentSection.h>
#include "HWUI.h"
class HWUIActions : public RPCActionManager, public ContentSection
{
 public:
  HWUIActions(UpdateDocumentContributor* parent, HWUI& hwui, EditBuffer& eb);
  Glib::ustring getPrefix() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring& path) override;
  bool handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request) override;
};
