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
  [[nodiscard]] Glib::ustring getPrefix() const override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring& path) override;

  template <typename Mgr> Mgr &findActionManager()
  {
    for(auto &a : m_actionManagers)
      if(auto m = std::dynamic_pointer_cast<Mgr>(a))
        return *m.get();

    throw std::runtime_error("ActionManager does not exist in object");
  }

 private:
  typedef std::shared_ptr<RPCActionManager> tRPCActionManagerPtr;
  std::list<tRPCActionManagerPtr> m_actionManagers;
};
