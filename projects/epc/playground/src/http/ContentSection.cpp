#include "ContentSection.h"
#include <device-settings/DebugLevel.h>

ContentSection::ContentSection(UpdateDocumentContributor *parent)
    : UpdateDocumentContributor(parent)
{
}

ContentSection::~ContentSection()
{
}

void ContentSection::handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring &path)
{
}

bool ContentSection::contains(const Glib::ustring &path) const
{
  auto prefix = getPrefix();
  auto found = path.find(prefix) == 1;
  return found;  // 1 for respecting the leading '/'
}
