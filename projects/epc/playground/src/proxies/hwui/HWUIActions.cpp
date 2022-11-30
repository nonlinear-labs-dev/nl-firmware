#include <http/HTTPRequest.h>
#include "HWUIActions.h"
#include "TestLayout.h"
#include <presets/PresetManager.h>
#include <libundo/undo/Scope.h>
#include <Application.h>
#include <use-cases/VoiceGroupAndLoadToPartManager.h>

HWUIActions::HWUIActions(UpdateDocumentContributor* parent, HWUI& hwui, EditBuffer& eb,
                         VoiceGroupAndLoadToPartManager& vgManager)
    : SectionAndActionManager(parent, "/hwui/")
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
              if(vgManager.getCurrentVoiceGroup() != part)
              {
                auto str = toString(part);
                auto scope = eb.getParent()->getUndoScope().startTransaction("Select Part " + str);
                vgManager.setCurrentVoiceGroupAndUpdateParameterSelection(scope->getTransaction(), part);
              }
            });

  addAction("inc-test-display",
            [&](const std::shared_ptr<NetworkRequest>& request)
            {
              auto& boled = hwui.getPanelUnit().getEditPanel().getBoled();
              auto& soled = hwui.getBaseUnit().getPlayPanel().getSOLED();
              auto panelTestLayout = std::dynamic_pointer_cast<TestLayout>(boled.getLayout());
              auto baseTestLayout = std::dynamic_pointer_cast<TestLayout>(soled.getLayout());
              if(panelTestLayout && baseTestLayout)
              {
                baseTestLayout->iterate();
                panelTestLayout->iterate();
              }
              else
              {
                boled.setOverlay(new TestLayout(boled));
                soled.setOverlay(new TestLayout(soled));
              }
            });
}
