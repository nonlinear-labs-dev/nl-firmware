#include "ActionManagers.h"

#include <presets/PresetManagerActions.h>
#include <presets/BankActions.h>
#include <presets/EditBufferActions.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUIActions.h>

ActionManagers::ActionManagers(UpdateDocumentContributor* parent, PresetManager& pm, AudioEngineProxy& aeProx,
                               HWUI& hwui)
    : ContentSection(parent)
{
  m_actionManagers.emplace_back(new PresetManagerActions(pm));
  m_actionManagers.emplace_back(new BankActions(pm));
  m_actionManagers.emplace_back(new EditBufferActions(*pm.getEditBuffer(), aeProx));
  m_actionManagers.emplace_back(new HWUIActions(parent, hwui, *pm.getEditBuffer()));
}

void ActionManagers::handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring& path)
{
  for(auto& a : m_actionManagers)
  {
    if(a->handleRequest(path, request))
      return;
  }
}

Glib::ustring ActionManagers::getPrefix() const
{
  return "";
}

void ActionManagers::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
