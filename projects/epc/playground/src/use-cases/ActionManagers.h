#pragma once
#include <http/ContentSection.h>
#include <http/RPCActionManager.h>
#include <list>
#include <http/SectionAndActionManager.h>

class PresetManager;
class AudioEngineProxy;
class HWUI;

class ActionManagers
{
 public:
  ActionManagers(UpdateDocumentContributor* parent, PresetManager& pm, AudioEngineProxy& aeProx,
                                    HWUI& hwui);


 private:
  typedef std::shared_ptr<SectionAndActionManager> tManagerPtr;
  std::list<tManagerPtr> m_actionManagers;

 public:
  std::list<tManagerPtr>& getManagers();
};
