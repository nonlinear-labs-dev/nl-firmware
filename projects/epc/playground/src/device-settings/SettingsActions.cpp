#include "SettingsActions.h"
#include "Settings.h"
#include "SyncSplitSettingUseCases.h"
#include <device-settings/DebugLevel.h>
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <http/NetworkRequest.h>
#include <proxies/hwui/TestLayout.h>
#include <use-cases/SettingsUseCases.h>
#include <tools/ExceptionTools.h>
#include <use-cases/DirectLoadUseCases.h>
#include <presets/PresetManager.h>
#include <nltools/messaging/Message.h>
#include <device-settings/Passphrase.h>
#include <http/HTTPRequest.h>

SettingsActions::SettingsActions(Settings& settings, PresetManager& pm)
    : super("/settings/")
{
  addAction("set-setting", [&](const std::shared_ptr<NetworkRequest>& request) {
    Glib::ustring key = request->get("key");
    Glib::ustring value = request->get("value");
    SettingsUseCases useCases(settings);
    useCases.setSettingFromWebUI(key, value, pm);
  });

  //TODO move into C15-Specific-Actions

  addAction("set-direct-load-with-load-to-part", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto pm = Application::get().getPresetManager();
    auto state = request->get("state");
    auto presetIfInLoadToPart = request->get("preset");
    auto fromIfInLoadToPart = request->get("from");
    auto totIfInLoadToPart = request->get("to");

    DirectLoadUseCases useCase(settings.getSetting<DirectLoadSetting>().get());

    try {
      if(state == "on") {
        if(auto preset = pm->findPreset(Uuid{presetIfInLoadToPart})) {
          useCase.enableDirectLoadFromWebUI(preset, to<VoiceGroup>(fromIfInLoadToPart), to<VoiceGroup>(totIfInLoadToPart));
        } else {
          useCase.enableDirectLoadWithoutPreset();
        }
      } else if(state == "off") {
        useCase.disableDirectLoad();
      }
    } catch(const std::runtime_error& e) {
      nltools::Log::error("Catched Error in \"set-direct-load-with-load-to-part\":", e.what());
    }
  });

  addAction("set-direct-load-without-load-to-part", [&](const std::shared_ptr<NetworkRequest>& request) {
    DirectLoadUseCases useCase(settings.getSetting<DirectLoadSetting>().get());
    auto state = request->get("state") == "on";
    useCase.setDirectLoad(state);
  });

  addAction("default-high-res", [&](auto request) {
    SettingsUseCases useCase(settings);
    useCase.setMappingsToHighRes();
  });

  addAction("default-classic-midi", [&](auto request) {
    SettingsUseCases useCase(settings);
    useCase.setMappingsToClassicMidi();
  });

  addAction("set-routing-aspect", [&](auto request) {
    try
    {
      auto hw = std::stoi(request->get("routing-entry"));
      auto aspect = std::stoi(request->get("aspect"));
      auto value = request->get("value") == "1";

      SettingsUseCases useCase(settings);
      useCase.updateRoutingAspect(hw, aspect, value);
    }
    catch(...)
    {
      ExceptionTools::errorLogCurrentException();
    }
   });

  addAction("panic-audio-engine", [](auto request) {
    SettingsUseCases::panicAudioEngine();
  });

  addAction("set-all-routings-to-value", [&](auto request) {
    auto requestedState = request->get("state") == "1";
    SettingsUseCases useCase(settings);
    useCase.setAllRoutingEntries(requestedState);
    nltools::Log::error(ExceptionTools::handle_eptr(std::current_exception()));
  });

  addAction("enable-bbb-wifi-for-epc2", [](auto) {
    nltools::msg::Setting::EnableBBBWifiFromDevSettings msg{};
    nltools::msg::send(nltools::msg::EndPoint::BeagleBone, msg);
  });

  addAction("is-valid-passphrase", [](auto request) {
    if(auto http = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto passphrase = http->get("text");
      if(!passphrase.empty())
      {
        auto val = Passphrase::isValidPassword(passphrase);
        http->setStatusOK();
        http->respond(val ? "1" : "0");
        http->okAndComplete();
      }
    }
  });

  addAction("dice-passphrase", [&](auto) {
    SettingsUseCases useCases(settings);
    useCases.dicePassphrase();
  });

  addAction("default-passphrase", [&](auto) {
    SettingsUseCases useCases(settings);
    useCases.defaultPassphrase();
  });
}

SettingsActions::~SettingsActions() = default;
