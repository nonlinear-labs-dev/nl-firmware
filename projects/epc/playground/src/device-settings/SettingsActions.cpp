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
#include <use-cases/DirectLoadUseCases.h>
#include <presets/PresetManager.h>

SettingsActions::SettingsActions(Settings &settings)
    : super("/settings/")
{
  addAction("set-setting", [&](std::shared_ptr<NetworkRequest> request) {
    Glib::ustring key = request->get("key");
    Glib::ustring value = request->get("value");

    DebugLevel::warning("Setting: ", key, " changed to: ", value);

    if(key == "SyncSplit")
    {
      auto useCases = SyncSplitSettingUseCases::get();
      useCases.updateFromWebUI(value);
      return;
    }

    if(auto s = settings.getSetting(key))
    {
      s->setSetting(Initiator::EXPLICIT_WEBUI, value);
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

  addAction("set-direct-load", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto state = request->get("state");
    auto presetIfInLoadToPart = request->get("preset");
    auto fromIfInLoadToPart = request->get("from");
    auto totIfInLoadToPart = request->get("to");

    DirectLoadUseCases useCase(settings.getSetting<DirectLoadSetting>().get());

    if(state == "on") {
      auto pm = Application::get().getPresetManager();
      if(auto preset = pm->findPreset(Uuid{presetIfInLoadToPart})) {
        useCase.enableDirectLoadFromWebUI(preset, to<VoiceGroup>(fromIfInLoadToPart), to<VoiceGroup>(totIfInLoadToPart));
      } else {
        useCase.enableDirectLoadFromWebUI(nullptr, VoiceGroup::NumGroups, VoiceGroup::NumGroups);
      }
    } else if(state == "off") {
      useCase.disableDirectLoad();
    }
  });

  addAction("default-high-res", [&](auto request) {
      SettingsUseCases useCase(Application::get().getSettings());
      useCase.setMappingsToHighRes();
  });

  addAction("default-classic-midi", [&](auto request) {
    SettingsUseCases useCase(Application::get().getSettings());
    useCase.setMappingsToClassicMidi();
  });
}

SettingsActions::~SettingsActions()
{
}
