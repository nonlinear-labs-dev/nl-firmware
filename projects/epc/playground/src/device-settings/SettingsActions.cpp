#include "SettingsActions.h"
#include "Settings.h"
#include "Setting.h"
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
#include <device-settings/Passphrase.h>
#include <http/HTTPRequest.h>

SettingsActions::SettingsActions(Settings &settings)
    : super("/settings/")
{
  addAction("set-setting", [&](std::shared_ptr<NetworkRequest> request) {
    Glib::ustring key = request->get("key");
    Glib::ustring value = request->get("value");

    if(Application::exists() && !key.empty())
    {
      SettingsUseCases useCases(Application::get().getSettings());
      useCases.setSettingFromWebUI(key, value);
    }
  });

  addAction("inc-test-display", [&](std::shared_ptr<NetworkRequest> request) {
    auto &app = Application::get();
    auto &boled = app.getHWUI()->getPanelUnit().getEditPanel().getBoled();
    auto &soled = app.getHWUI()->getBaseUnit().getPlayPanel().getSOLED();
    auto panelTestLayout = dynamic_cast<TestLayout *>(boled.getLayout().get());
    auto baseTestLayout = dynamic_cast<TestLayout *>(soled.getLayout().get());

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
    SettingsUseCases useCase(Application::get().getSettings());
    useCase.setMappingsToHighRes();
  });

  addAction("default-classic-midi", [&](auto request) {
    SettingsUseCases useCase(Application::get().getSettings());
    useCase.setMappingsToClassicMidi();
  });

  addAction("hw-source-enable-set", [&](auto request) {
    try
    {
      auto hw = std::stoi(request->get("hw"));
      auto aspect = std::stoi(request->get("aspect"));
      auto value = request->get("value") == "1";

      SettingsUseCases useCase(Application::get().getSettings());
      useCase.updateHWSourceEnable(hw, aspect, value);
    }
    catch(...)
    {
      nltools::Log::error(ExceptionTools::handle_eptr(std::current_exception()));
    }
   });

  addAction("panic-audio-engine", [](auto request) {
    SettingsUseCases useCase(Application::get().getSettings());
    useCase.panicAudioEngine();
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

  addAction("dice-passphrase", [](auto) {
      if(Application::exists()) {
        SettingsUseCases useCases(Application::get().getSettings());
        useCases.dicePassphrase();
      }
  });

  addAction("default-passphrase", [](auto) {
    if(Application::exists()) {
      SettingsUseCases useCases(Application::get().getSettings());
      useCases.defaultPassphrase();
    }
  });
}

SettingsActions::~SettingsActions() = default;
