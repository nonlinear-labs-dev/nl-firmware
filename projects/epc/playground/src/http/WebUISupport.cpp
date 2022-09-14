#include "WebUISupport.h"
#include <device-settings/PedalType.h>
#include <xml/OutStream.h>
#include <http/NetworkRequest.h>
#include <http/HTTPRequest.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/flac/AutoStartRecorderSetting.h>
#include <device-settings/flac/FlacRecorderVirgin.h>

namespace
{
  template <typename T> std::string getEnumStrings()
  {
    std::string result;
    auto names = Application::get().getSettings()->getSetting<PedalType>()->getDisplayStrings();

    for(const auto& name : names)
      result.append(result.empty() ? 0 : 1, '\n').append(name);

    return result;
  }
}

WebUISupport::WebUISupport(UpdateDocumentContributor* master)
    : SectionAndActionManager(master, "/webui-support/")
{
  addAction("enum-get-strings", [&](const std::shared_ptr<NetworkRequest>& request) {
    if(auto h = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      h->setStatusOK();

      if(h->get("name") == "PedalTypes")
        h->respond(getEnumStrings<PedalTypes>());

      h->okAndComplete();
    }
  });

  addAction("show-recorder-autostart-message", [&](const std::shared_ptr<NetworkRequest>& request) {
    if(auto h = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto autoStartEnabled = Application::get().getSettings()->getSetting<AutoStartRecorderSetting>()->get();
      auto recorderIsVirgin = Application::get().getSettings()->getSetting<FlacRecorderVirgin>()->get();
      auto showMessage = recorderIsVirgin and not autoStartEnabled;

      h->setStatusOK();
      h->respond(std::to_string(showMessage));
      h->okAndComplete();
    }
  });
}

void WebUISupport::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID) const
{
  writer.writeTag("webui-helper", [&]() {
    auto selectedVG = Application::get().getVGManager()->getCurrentVoiceGroup();
    writer.writeTextElement("selected-part", toString(selectedVG));
  });
}
