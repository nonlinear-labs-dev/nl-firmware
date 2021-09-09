#include <http/HTTPRequest.h>
#include "HWUIActions.h"
#include <presets/PresetManager.h>
#include <libundo/undo/Scope.h>

HWUIActions::HWUIActions(UpdateDocumentContributor* parent, HWUI& hwui, EditBuffer& eb)
    : RPCActionManager("/hwui/")
    , ContentSection(parent)
{
  addAction("download-soled-as-png",
            [&](const std::shared_ptr<NetworkRequest>& request)
            {
              if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
              {
                httpRequest->respond(hwui.exportSoled());
              }
            });

  addAction("download-boled-as-png",
            [&](auto request)
            {
              if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
              {
                httpRequest->respond(hwui.exportBoled());
              }
            });

  addAction("select-part-from-webui",
            [&](const std::shared_ptr<NetworkRequest>& request)
            {
              auto part = to<VoiceGroup>(request->get("part"));
              if(hwui.getCurrentVoiceGroup() != part && eb.isDual())
              {
                auto str = toString(part);
                auto scope = eb.getParent()->getUndoScope().startTransaction("Select Part " + str);
                hwui.setCurrentVoiceGroupAndUpdateParameterSelection(scope->getTransaction(), part);
              }
            });
}

void HWUIActions::handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring& path)
{
  handleRequest(path, request);
}

bool HWUIActions::handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request)
{
  return RPCActionManager::handleRequest(path, request);
}

Glib::ustring HWUIActions::getPrefix() const
{
  return "/hwui/";
}

void HWUIActions::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}
