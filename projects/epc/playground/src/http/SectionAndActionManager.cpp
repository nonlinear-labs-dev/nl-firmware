#include "SectionAndActionManager.h"

SectionAndActionManager::SectionAndActionManager(UpdateDocumentContributor *parent, const Glib::ustring &basePath)
    : ContentSection(parent)
    , RPCActionManager(basePath)
{
}

Glib::ustring SectionAndActionManager::getPrefix() const
{
  return RPCActionManager::getBasePath().substr(1);
}

void SectionAndActionManager::handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring &path)
{
  RPCActionManager::handleRequest(request);
}

void SectionAndActionManager::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
