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
#include <xml/MemoryInStream.h>
#include <xml/OutStream.h>
#include <xml/VersionAttribute.h>
#include <boost/algorithm/string.hpp>
#include <tools/StringTools.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <tools/TimeTools.h>
#include <proxies/hwui/panel-unit/boled/setup/ExportBackupEditor.h>
#include <device-settings/DebugLevel.h>
#include <nltools/Assert.h>
#include <Application.h>
#include <presets/SendEditBufferScopeGuard.h>

PresetManagerActions::PresetManagerActions(PresetManager &presetManager)
    : RPCActionManager("/presets/")
    , m_presetManager(presetManager)
    , pmUseCases { &m_presetManager }
    , soundUseCases { m_presetManager.getEditBuffer(), &m_presetManager }
{
  addAction("new-bank", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto x = request->get("x");
    auto y = request->get("y");
    auto name = request->get("name");
    pmUseCases.newBank(x, y, name);
  });

  addAction("new-bank-from-edit-buffer", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto x = request->get("x");
    auto y = request->get("y");
    pmUseCases.newBank(x, y);
  });

  addAction("rename-bank", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto uuid = request->get("uuid");
    auto newName = request->get("name");
    if(auto b = m_presetManager.findBank(uuid))
    {
      BankUseCases useCase(b);
      useCase.renameBank(newName);
    }
  });

  addAction("select-bank", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto uuid = request->get("uuid");
    pmUseCases.selectBank(Uuid(uuid));
  });

  addAction("delete-bank", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto uuid = request->get("uuid");
    pmUseCases.deleteBank(Uuid(uuid));
  });

  addAction("find-unique-preset-name", [&](std::shared_ptr<NetworkRequest> request) mutable {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      Glib::ustring basedOn = request->get("based-on");
      Glib::ustring result = presetManager.createPresetNameBasedOn(basedOn);
      httpRequest->respond(result);
    }
  });

  addAction("store-init", [&](std::shared_ptr<NetworkRequest>) mutable { soundUseCases.storeInitSound(); });

  addAction("reset-init", [&](std::shared_ptr<NetworkRequest>) mutable { soundUseCases.resetInitSound(); });

  addAction("import-all-banks", [&](std::shared_ptr<NetworkRequest> request) mutable {
    if(auto http = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto &boled = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
      auto scope = presetManager.getUndoScope().startTransaction("Import all Banks");
      auto transaction = scope->getTransaction();

      boled.setOverlay(new SplashLayout());
      auto *buffer = http->getFlattenedBuffer();

      handleImportBackupFile(transaction, buffer, http);

      boled.resetOverlay();
      soup_buffer_free(buffer);
    }
  });

  addAction("load-preset-from-xml", [&](std::shared_ptr<NetworkRequest> request) mutable {
    if(auto http = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto editBuffer = presetManager.getEditBuffer();

      Preset p(editBuffer);

      {
        auto scope = presetManager.getUndoScope().startTrashTransaction();
        auto transaction = scope->getTransaction();
        auto xml = http->get("xml", "");

        MemoryInStream stream(xml, false);
        XmlReader reader(stream, transaction);

        if(!reader.read<PresetSerializer>(&p))
        {
          http->respond("Invalid File!");
          DebugLevel::warning("Could not read Preset xml!");
          return;
        }
      }

      auto loadscope = presetManager.getUndoScope().startTransaction("Load Compare Buffer");
      auto loadtransaction = loadscope->getTransaction();

      SendEditBufferScopeGuard scopeGuard(loadtransaction, true);

      auto autoLoadSetting = Application::get().getSettings()->getSetting<DirectLoadSetting>();
      auto scopedLock = autoLoadSetting->scopedOverlay(BooleanSettings::BOOLEAN_SETTING_FALSE);
      editBuffer->copyFrom(loadtransaction, &p);
      editBuffer->undoableSetLoadedPresetInfo(loadtransaction, &p);
    }
  });

  addAction("move-cluster", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto csv = request->get("csv");
    std::vector<std::string> uuids;
    boost::split(uuids, csv, boost::is_any_of(","));
    pmUseCases.moveBankCluster(uuids);
  });

  addAction("select-midi-bank", [&](std::shared_ptr<NetworkRequest> request) mutable {
    auto bankUuid = request->get("bank", "");
    auto bank = Application::get().getPresetManager()->findBank(bankUuid);
    pmUseCases.selectMidiBank(bank);
  });
}

void PresetManagerActions::handleImportBackupFile(UNDO::Transaction *transaction, SoupBuffer *buffer,
                                                  std::shared_ptr<HTTPRequest> http)
{
  if(auto lock = m_presetManager.getLoadingLock())
  {
    m_presetManager.clear(transaction);

    MemoryInStream stream(buffer, true);
    XmlReader reader(stream, transaction);

    if(!reader.read<PresetManagerSerializer>(&m_presetManager))
    {
      transaction->rollBack();
      http->respond("Invalid File. Please choose correct xml.tar.gz or xml.zip file.");
    }
    else
    {
      m_presetManager.getEditBuffer()->sendToAudioEngine();
    }
  }
}

bool PresetManagerActions::handleRequest(const Glib::ustring &path, std::shared_ptr<NetworkRequest> request)
{
  if(super::handleRequest(path, request))
    return true;

  if(path.find("/presets/search-preset") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      Glib::ustring query = request->get("query");
      Glib::ustring mode = request->get("combine");
      Glib::ustring field = request->get("fields");

      std::vector<SearchQuery::Fields> fields;

      auto splitFieldStrings = StringTools::splitStringOnAnyDelimiter(field, ',');
      std::for_each(splitFieldStrings.begin(), splitFieldStrings.end(), [&](const std::string &t) {
        if(t == "name")
          fields.push_back(SearchQuery::Fields::Name);
        else if(t == "comment")
          fields.push_back(SearchQuery::Fields::Comment);
        else if(t == "devicename")
          fields.push_back(SearchQuery::Fields::DeviceName);
      });

      XmlWriter writer(request->createStream("text/xml", false));
      Application::get().getPresetManager()->searchPresets(writer, query, mode, std::move(fields));
      return true;
    }
  }

  if(path.find("/presets/download-banks") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto &boled = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
      boled.setOverlay(new SplashLayout());

      const auto time = TimeTools::getDisplayStringFromStamp(TimeTools::getAdjustedTimestamp());
      const auto timeWithoutWhitespaces = StringTools::replaceAll(time, " ", "-");
      const auto timeSanitized = StringTools::replaceAll(timeWithoutWhitespaces, ":", "-");
      httpRequest->setHeader("Content-Disposition",
                             "attachment; filename=\"" + timeSanitized + "-nonlinear-c15-banks.xml.tar.gz\"");
      ExportBackupEditor::writeBackupToStream(request->createStream("application/zip", true));

      boled.resetOverlay();
      return true;
    }
  }

  if(path.find("/presets/get-diff") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      auto pm = Application::get().getPresetManager();
      auto eb = pm->getEditBuffer();
      auto ebAsPreset = std::make_unique<Preset>(pm, *eb, true);
      auto aUUID = request->get("p1");
      auto bUUID = request->get("p2");
      auto voiceGroupOfA = request->get("vg1");
      auto voiceGroupOfB = request->get("vg2");
      auto a = pm->findPreset(aUUID);
      auto b = pm->findPreset(bUUID);
      a = a ? a : ebAsPreset.get();
      b = b ? b : ebAsPreset.get();
      XmlWriter writer(request->createStream("text/xml", false));
      a->writeDiff(writer, b, to<VoiceGroup>(voiceGroupOfA), to<VoiceGroup>(voiceGroupOfB));
      return true;
    }
  }

  return false;
}
