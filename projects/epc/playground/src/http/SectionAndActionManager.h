#pragma once
#include <http/ContentSection.h>
#include <http/RPCActionManager.h>

class SectionAndActionManager : public ContentSection, public RPCActionManager
{
 public:
  SectionAndActionManager(UpdateDocumentContributor *parent, const Glib::ustring &basePath);
  [[nodiscard]] Glib::ustring getPrefix() const override;
  void handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring &path) override;
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
};
