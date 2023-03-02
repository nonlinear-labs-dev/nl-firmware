#include <presets/PresetManagerActions.h>
#include <http/HTTPRequest.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <serialization/PresetManagerSerializer.h>
#include <serialization/PresetSerializer.h>
#include <xml/ZippedMemoryOutStream.h>
#include <xml/OutStream.h>
#include <boost/algorithm/string.hpp>
#include <tools/StringTools.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <tools/TimeTools.h>
#include <proxies/hwui/panel-unit/boled/setup/ExportBackupEditor.h>
#include <device-settings/DebugLevel.h>
#include <Application.h>
#include <device-settings/DateTimeAdjustment.h>
#include "use-cases/SplashScreenUseCases.h"
#include <presets/PresetParameter.h>
#include <parameters/presenter-rules/ParameterPresenterRules.h>

PresetManagerActions::PresetManagerActions(UpdateDocumentContributor* parent, PresetManager& presetManager,
                                           AudioEngineProxy& aeProxy, Settings& settings)
    : SectionAndActionManager(parent, "/presets/")
    , m_presetManager { presetManager }
    , m_settings { settings }
    , m_aeProxy { aeProxy }
{
  addAction("new-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto x = request->get("x");
    auto y = request->get("y");
    auto name = request->get("name");
    PresetManagerUseCases useCase { m_presetManager, m_settings };
    useCase.newBank(x, y, name);
  });

  addAction("new-bank-from-edit-buffer", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto x = request->get("x");
    auto y = request->get("y");
    auto uuid = request->get("uuid");
    PresetManagerUseCases useCase { m_presetManager, m_settings };
    useCase.newBank(x, y, Uuid { uuid });
  });

  addAction("rename-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("uuid");
    auto newName = request->get("name");
    if(auto b = m_presetManager.findBank(Uuid { uuid }))
    {
      BankUseCases useCase(b, m_settings);
      useCase.renameBank(newName);
    }
  });

  addAction("select-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("uuid");
    PresetManagerUseCases useCase { m_presetManager, m_settings };
    useCase.selectBank(Uuid(uuid));
  });

  addAction("delete-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("uuid");
    PresetManagerUseCases useCase { m_presetManager, m_settings };
    useCase.deleteBank(Uuid(uuid));
  });

  addAction("find-unique-preset-name", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      Glib::ustring basedOn = request->get("based-on");
      Glib::ustring result = presetManager.createPresetNameBasedOn(basedOn);
      httpRequest->respond(result);
    }
  });

  addAction("store-init", [&](const auto&) mutable {
    SoundUseCases useCase(m_presetManager.getEditBuffer(), &m_presetManager);
    useCase.storeInitSound();
  });

  addAction("reset-init", [&](const auto&) mutable {
    SoundUseCases useCase(m_presetManager.getEditBuffer(), &m_presetManager);
    useCase.resetInitSound();
  });

  addAction("import-all-banks", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    if(auto http = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto* buffer = http->getFlattenedBuffer();

      PresetManagerUseCases useCase(m_presetManager, m_settings);
      auto hwui = Application::get().getHWUI();
      auto* settings = &m_settings;

      auto start = [hwui, settings]() {
        SplashScreenUseCases ssuc(*hwui, *settings);
        ssuc.startSplashScreen();
      };

      auto addStatus = [hwui, settings](auto str) {
        SplashScreenUseCases ssuc(*hwui, *settings);
        ssuc.addSplashScreenMessage(str);
      };

      auto finish = [hwui, settings]() {
        SplashScreenUseCases ssuc(*hwui, *settings);
        ssuc.finishSplashScreen();
      };

      auto ret = useCase.importBackupFile(buffer, { start, addStatus, finish }, m_aeProxy);
      http->respond(PresetManagerUseCases::exitCodeToErrorMessage(ret));
      soup_buffer_free(buffer);
    }
  });

  addAction("load-preset-from-xml", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    if(auto http = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto xml = http->get("xml", "");
      PresetManagerUseCases useCase(m_presetManager, m_settings);
      if(!useCase.loadPresetFromCompareXML(xml))
      {
        http->respond("Invalid File!");
      }
    }
  });

  addAction("move-cluster", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto csv = request->get("csv");
    std::vector<std::string> uuids;
    boost::split(uuids, csv, boost::is_any_of(","));
    PresetManagerUseCases useCase { m_presetManager, m_settings };
    useCase.moveBankCluster(uuids);
  });

  addAction("select-midi-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto bankUuid = request->get("bank", "");
    auto bank = Application::get().getPresetManager()->findBank(Uuid { bankUuid });
    PresetManagerUseCases useCase { m_presetManager, m_settings };
    useCase.selectMidiBank(bank);
  });

  addAction("isFXUnused", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    if(auto http = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto presetUUID = request->get("uuid", "");
      if(auto preset = Application::get().getPresetManager()->findPreset(Uuid { presetUUID }))
      {
        auto fxI_unused = ParameterPresenterRules::isPresetFXUnused_I(preset);
        auto fxII_unused = ParameterPresenterRules::isPresetFXUnused_II(preset);
        if(fxI_unused && fxII_unused)
          http->respond("both");
        else if(fxI_unused)
          http->respond("I");
        else if(fxII_unused)
          http->respond("II");
        else
          http->respond("none");
      }
    }
  });
}

bool PresetManagerActions::handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request)
{
  if(RPCActionManager::handleRequest(path, request))
    return true;

  if(path.find("/presets/download-banks") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto hwui = Application::get().getHWUI();
      auto settings = Application::get().getSettings();

      SplashScreenUseCases ssuc(*hwui, *settings);
      ssuc.startSplashScreen();

      const auto time = TimeTools::getDisplayStringFromStamp(
          TimeTools::getAdjustedTimestamp(settings->getSetting<DateTimeAdjustment>()));
      const auto timeWithoutWhitespaces = StringTools::replaceAll(time, " ", "-");
      const auto timeSanitized = StringTools::replaceAll(timeWithoutWhitespaces, ":", "-");
      auto disposition = "attachment; filename=\"" + timeSanitized + "-nonlinear-c15-banks.xml.tar.gz\"";
      ZippedMemoryOutStream stream;
      ExportBackupEditor::writeBackupToStream(stream);
      httpRequest->respondComplete(SOUP_STATUS_OK, "application/zip", { { "Content-Disposition", disposition } },
                                   stream.exhaust());
      ssuc.finishSplashScreen();
      return true;
    }
  }

  if(path.find("/presets/get-diff") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto pm = Application::get().getPresetManager();
      auto eb = pm->getEditBuffer();
      auto ebAsPreset = std::make_unique<Preset>(pm, *eb);
      auto aUUID = request->get("p1");
      auto bUUID = request->get("p2");
      auto voiceGroupOfA = request->get("vg1");
      auto voiceGroupOfB = request->get("vg2");
      auto a = pm->findPreset(Uuid { aUUID });
      auto b = pm->findPreset(Uuid { bUUID });
      a = a ? a : ebAsPreset.get();
      b = b ? b : ebAsPreset.get();
      auto stream = request->createStream("text/xml", false);
      XmlWriter writer(*stream);
      a->writeDiff(writer, b, to<VoiceGroup>(voiceGroupOfA), to<VoiceGroup>(voiceGroupOfB));
      return true;
    }
  }

  return false;
}

void PresetManagerActions::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  m_presetManager.writeDocument(writer, knownRevision);
}
