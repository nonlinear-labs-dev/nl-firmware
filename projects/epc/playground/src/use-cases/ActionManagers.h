#pragma once
#include <http/ContentSection.h>
#include <http/RPCActionManager.h>
#include <list>

class PresetManager;
class AudioEngineProxy;
class HWUI;

class ActionManagers : public ContentSection
{
 public:
  ActionManagers(UpdateDocumentContributor* parent, PresetManager& pm, AudioEngineProxy& aeProx,
                                    HWUI& hwui);
  Glib::ustring getPrefix() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring& path) override;

 private:
  typedef std::shared_ptr<RPCActionManager> tRPCActionManagerPtr;
  std::list<tRPCActionManagerPtr> m_actionManagers;
};
