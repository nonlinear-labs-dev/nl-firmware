#include "ActionManagers.h"

#include <presets/PresetManagerActions.h>
#include <presets/BankActions.h>
#include <presets/EditBufferActions.h>
#include <presets/PresetManager.h>

#include <proxies/hwui/HWUIActions.h>

#include <device-settings/Settings.h>

ActionManagers::ActionManagers(UpdateDocumentContributor* parent, PresetManager& pm, AudioEngineProxy& aeProx,
                               HWUI& hwui, Settings& settings)
    : ContentSection(parent)
{
  m_actionManagers.emplace_back(new PresetManagerActions(pm, aeProx));
  m_actionManagers.emplace_back(new BankActions(pm));
  m_actionManagers.emplace_back(new EditBufferActions(*pm.getEditBuffer(), aeProx, settings));
  m_actionManagers.emplace_back(new HWUIActions(hwui, *pm.getEditBuffer()));
}

Glib::ustring ActionManagers::getPrefix() const
{
  return "presets";
}

void ActionManagers::writeDocument(Writer& writer,
                                                      UpdateDocumentContributor::tUpdateID knownRevision) const
{
  nltools::Log::error("Not Implemented!");
}

void ActionManagers::handleHTTPRequest(std::shared_ptr<NetworkRequest> request,
                                                          const Glib::ustring& path)
{
  ContentSection::handleHTTPRequest(request, path);

  for(auto& actionManager : m_actionManagers)
    if(actionManager->matches(path))
      if(actionManager->handleRequest(path, request))
        return;
}
