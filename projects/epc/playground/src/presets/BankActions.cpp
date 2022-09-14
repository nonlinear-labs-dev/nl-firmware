#include "BankActions.h"

#include <use-cases/EditBufferUseCases.h>
#include <use-cases/PresetManagerUseCases.h>
#include <use-cases/PresetUseCases.h>

#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetManager.h>

#include <http/NetworkRequest.h>
#include <http/HTTPRequest.h>

#include <boost/algorithm/string.hpp>

#include <xml/MemoryInStream.h>
#include <xml/MemoryOutStream.h>
#include <xml/VersionAttribute.h>
#include <serialization/PresetSerializer.h>

#include <tools/TimeTools.h>

#include <device-settings/Settings.h>
#include <device-settings/DateTimeAdjustment.h>

BankActions::BankActions(UpdateDocumentContributor* parent, PresetManager& presetManager, Settings& settings)
    : SectionAndActionManager(parent, "/banks/")
    , m_presetManager(presetManager)
    , m_settings{settings}
{
  addAction("drop-presets-above", [&](const std::shared_ptr<NetworkRequest>& request) {
    Glib::ustring csv = request->get("presets");
    Glib::ustring anchorUUID = request->get("anchor");

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.dropPresets(anchorUUID, PresetManagerUseCases::DropActions::Above, csv);
  });

  addAction("drop-presets-below", [&](const std::shared_ptr<NetworkRequest>& request) {
    Glib::ustring csv = request->get("presets");
    Glib::ustring anchorUUID = request->get("anchor");

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.dropPresets(anchorUUID, PresetManagerUseCases::DropActions::Below, csv);
  });

  addAction("drop-presets-to", [&](const std::shared_ptr<NetworkRequest>& request) {
    Glib::ustring csv = request->get("presets");
    Glib::ustring anchorUUID = request->get("anchor");

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.dropPresets(anchorUUID, PresetManagerUseCases::DropActions::Onto, csv);
  });

  addAction("rename-preset", [&](const std::shared_ptr<NetworkRequest>& request) {
    Glib::ustring uuid = request->get("uuid");
    Glib::ustring newName = request->get("name");

    if(auto p = m_presetManager.findPreset(Uuid { uuid }))
    {
      PresetUseCases useCase(*p, m_settings);
      useCase.rename(newName);
    }
  });

  addAction("move-preset-above", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto presetToMoveUuid = request->get("presetToMove");
    auto presetAnchorUuid = request->get("anchor");

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.movePresetAbove(Uuid { presetToMoveUuid }, Uuid { presetAnchorUuid });
  });

  addAction("move-preset-below", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetToMoveUuid = request->get("presetToMove");
    auto presetAnchorUuid = request->get("anchor");

    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.movePresetBelow(Uuid { presetToMoveUuid }, Uuid { presetAnchorUuid });
  });

  addAction("move-preset-to", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetToOverwrite = request->get("presetToOverwrite");
    auto overwriteWith = request->get("overwriteWith");
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.movePresetTo(Uuid { overwriteWith }, Uuid { presetToOverwrite });
  });

  addAction("overwrite-preset", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetToOverwrite = request->get("presetToOverwrite");
    auto overwriteWith = request->get("overwriteWith");

    auto sourcePreset = m_presetManager.findPreset(Uuid { overwriteWith });
    auto targetPreset = m_presetManager.findPreset(Uuid { presetToOverwrite });

    PresetManagerUseCases useCases(m_presetManager, m_settings);

    if(targetPreset)
    {
      PresetUseCases targetUseCases(*targetPreset, m_settings);
      if(sourcePreset)
      {
        targetUseCases.overwriteWithPreset(sourcePreset);
      }
      else
      {
        targetUseCases.overwriteWithEditBuffer(*m_presetManager.getEditBuffer());
      }
    }
    else
    {
      useCases.createBankAndStoreEditBuffer();
    }
  });

  addAction("copy-preset-above", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetToCopy = request->get("presetToCopy");
    auto presetAnchor = request->get("anchor");
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.copyPresetAbove(Uuid { presetToCopy }, Uuid { presetAnchor });
  });

  addAction("copy-preset-below", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetToCopy = request->get("presetToCopy");
    auto presetAnchor = request->get("anchor");
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.copyPresetBelow(Uuid { presetToCopy }, Uuid { presetAnchor });
  });

  addAction("insert-editbuffer-above", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetAnchor = request->get("anchor");
    auto uuid = request->get("uuid");
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.insertEditBufferAbove(Uuid { presetAnchor }, Uuid { uuid });
  });

  addAction("insert-editbuffer-below", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetAnchor = request->get("anchor");
    auto uuid = request->get("uuid");
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.insertEditBufferBelow(Uuid { presetAnchor }, Uuid { uuid });
  });

  addAction("overwrite-preset-with-editbuffer", [&](const std::shared_ptr<NetworkRequest>& request) {
    auto presetToOverwrite = request->get("presetToOverwrite");
    PresetUseCases useCase(*m_presetManager.findPreset(Uuid{ presetToOverwrite }), m_settings);
    useCase.overwriteWithEditBuffer(*m_presetManager.getEditBuffer());
  });

  addAction("append-preset", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto b = m_presetManager.getSelectedBank();
    auto fallBack = b ? b->getUuid().raw() : "";
    auto bankToAppendTo = request->get("bank-uuid", fallBack);
    auto uuid = request->get("uuid");

    if(auto bank = m_presetManager.findBank(Uuid { bankToAppendTo }))
    {
      BankUseCases bankUseCases(bank, m_settings);
      bankUseCases.appendEditBufferAsPresetWithUUID(Uuid { uuid });
    }
  });

  addAction("append-preset-to-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto bankUuid = request->get("bank-uuid");
    auto presetUuid = request->get("preset-uuid");

    if(auto bank = m_presetManager.findBank(Uuid { bankUuid }))
    {
      if(auto srcPreset = m_presetManager.findPreset(Uuid { presetUuid }))
      {
        PresetManagerUseCases useCases(m_presetManager, m_settings);
        useCases.appendPreset(bank, srcPreset);
      }
    }
  });

  addAction("set-order-number", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("uuid");
    auto orderNumber = std::max(1ULL, std::stoull(request->get("order-number"))) - 1;

    if(auto bank = m_presetManager.findBank(Uuid { uuid }))
    {
      PresetManagerUseCases useCase(m_presetManager, m_settings);
      useCase.setOrderNumber(bank, static_cast<int>(orderNumber));
    }
  });

  addAction("insert-preset", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto selUuid = request->get("seluuid");
    auto newUuid = request->get("uuid");

    if(auto bank = m_presetManager.findBankWithPreset(Uuid { selUuid }))
    {
      BankUseCases bankUseCases(bank, m_settings);
      auto desiredPresetPos = bank->getPresetPosition(Uuid { selUuid }) + 1;
      bankUseCases.insertEditBufferAsPresetWithUUID(desiredPresetPos, Uuid { newUuid });
    }
  });

  addAction("select-preset", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    Glib::ustring presetUUID = request->get("uuid");
    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.selectPreset(Uuid { presetUUID });
  });

  addAction("delete-preset", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto presetUUID = request->get("uuid");
    auto deleteBank = request->get("delete-bank") == "true";

    if(auto srcBank = m_presetManager.findBankWithPreset(Uuid { presetUUID }))
    {
      if(deleteBank)
      {
        PresetManagerUseCases useCase(m_presetManager, m_settings);
        useCase.deleteBank(srcBank);
      }
      else
      {
        BankUseCases useCae(srcBank, m_settings);
        useCae.deletePreset(Uuid { presetUUID });
      }
    }
  });

  addAction("delete-presets", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    std::vector<std::string> strs;
    auto csv = request->get("presets");
    auto deleteBanks = request->get("delete-bank") == "true";
    boost::split(strs, csv, boost::is_any_of(","));

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.deletePresets(strs, deleteBanks);
  });

  addAction("load-preset", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("uuid");
    EditBufferUseCases useCase(*m_presetManager.getEditBuffer());
    useCase.load(Uuid { uuid });
  });

  addAction("set-position", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("uuid");
    auto x = request->get("x");
    auto y = request->get("y");
    if(auto bank = m_presetManager.findBank(Uuid { uuid }))
    {
      BankUseCases useCase(bank, m_settings);
      useCase.moveBank(x, y);
    }
  });

  addAction("create-new-bank-from-preset", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto uuid = request->get("preset");
    auto x = request->get("x");
    auto y = request->get("y");

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.createBankFromPreset(Uuid { uuid }, x, y);
  });

  addAction("create-new-bank-from-presets", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto csv = request->get("presets");
    auto x = request->get("x");
    auto y = request->get("y");

    PresetManagerUseCases useCase(m_presetManager, m_settings);
    useCase.createBankFromPresets(csv, x, y);
  });

  addAction("drop-bank-on-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    Glib::ustring receiverUuid = request->get("receiver");
    Glib::ustring bankUuid = request->get("bank");

    if(auto tgtBank = m_presetManager.findBank(Uuid { receiverUuid }))
    {
      if(auto srcBank = m_presetManager.findBank(Uuid { bankUuid }))
      {
        BankUseCases useCase(tgtBank, m_settings);
        useCase.dropBank(srcBank);
      }
    }
  });

  addAction("drop-presets-on-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    Glib::ustring bankUUID = request->get("bank");
    Glib::ustring csv = request->get("presets");

    if(auto tgtBank = m_presetManager.findBank(Uuid { bankUUID }))
    {
      BankUseCases useCase(tgtBank, m_settings);
      useCase.dropPresets(csv);
    }
  });

  addAction("insert-bank-above", [&](const std::shared_ptr<NetworkRequest>& request) mutable { insertBank(request, 0); });

  addAction("insert-bank-below", [&](const std::shared_ptr<NetworkRequest>& request) mutable { insertBank(request, 1); });

  addAction("overwrite-preset-with-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto anchorUuid = request->get("anchor");
    auto bankUuid = request->get("bank");
    if(auto srcbank = m_presetManager.findBank(Uuid { bankUuid }))
    {
      if(auto tgtBank = m_presetManager.findBankWithPreset(Uuid { anchorUuid }))
      {
        BankUseCases useCase(tgtBank, m_settings);
        useCase.dropBankOnPreset(srcbank, Uuid { anchorUuid });
      }
    }
  });

  addAction("import-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto xml = request->get("xml");
    auto x = request->get("x");
    auto y = request->get("y");
    auto fileName = request->get("fileName");
    MemoryInStream stream(xml, false);

    //TODO inject proper Progress CB
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.importBankFromStream(stream, std::stoi(x), std::stoi(y), fileName, [](auto){});
  });

  addAction("set-preset-attribute", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto presetUUID = request->get("uuid");
    auto key = request->get("key");
    auto value = request->get("value");

    if(auto preset = m_presetManager.findPreset(Uuid { presetUUID }))
    {
      PresetUseCases useCase(*preset, m_settings);
      useCase.setAttribute(key, value);
    }
  });

  addAction("set-bank-attribute", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto bankUUID = request->get("uuid");
    auto key = request->get("key");
    auto value = request->get("value");

    if(auto bank = m_presetManager.findBank(Uuid { bankUUID }))
    {
      BankUseCases useCase(bank, m_settings);
      useCase.setAttribute(key, value);
    }
  });

  addAction("set-bank-collapse", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto bankUUID = request->get("uuid");
    auto value = request->get("value");

    if(auto bank = m_presetManager.findBank(Uuid { bankUUID }))
    {
      BankUseCases useCase(bank, m_settings);
      useCase.setCollapsed(value == "true");
    }
  });

  addAction("move", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto bankUUID = request->get("bank");
    auto value = request->get("direction");

    PresetManagerUseCases useCase(m_presetManager, m_settings);

    if(auto bank = m_presetManager.getSelectedBank())
    {
      if(value == "LeftByOne")
        useCase.moveLeft(bank);
      else if(value == "RightByOne")
        useCase.moveRight(bank);
    }
  });

  addAction("insert-bank-in-cluster", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    const auto insertedUUID = request->get("bank-to-insert");
    const auto insertedAtUUID = request->get("bank-inserted-at");
    const auto orientation = request->get("orientation");

    PresetManagerUseCases useCase(m_presetManager, m_settings);

    if(auto bankToInsert = m_presetManager.findBank(Uuid { insertedUUID }))
    {
      if(auto bankAtInsertPosition = m_presetManager.findBank(Uuid { insertedAtUUID }))
      {
        useCase.insertBankInCluster(bankToInsert, bankAtInsertPosition, orientation);
      }
    }
  });

  addAction("dock-banks", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    const auto droppedOntoBankUuid = request->get("droppedOntoBank");
    const auto draggedBankUuid = request->get("draggedBank");
    const auto droppedAt = request->get("droppedAt");
    const auto x = request->get("x");
    const auto y = request->get("y");

    if(auto droppedOntoBank = m_presetManager.findBank(Uuid { droppedOntoBankUuid }))
    {
      if(auto draggedBank = m_presetManager.findBank(Uuid { draggedBankUuid }))
      {
        auto scope = presetManager.getUndoScope().startTransaction(
            "Dock Banks '%0' and '%1'", droppedOntoBank->getName(true), draggedBank->getName(true));
        auto transaction = scope->getTransaction();

        if(droppedAt == "North")
        {
          droppedOntoBank->attachBank(transaction, draggedBank->getUuid(), Bank::AttachmentDirection::top);
        }
        else if(droppedAt == "West")
        {
          droppedOntoBank->attachBank(transaction, draggedBank->getUuid(), Bank::AttachmentDirection::left);
        }
        else if(droppedAt == "South")
        {
          draggedBank->attachBank(transaction, droppedOntoBank->getUuid(), Bank::AttachmentDirection::top);
        }
        else if(droppedAt == "East")
        {
          draggedBank->attachBank(transaction, droppedOntoBank->getUuid(), Bank::AttachmentDirection::left);
        }

        draggedBank->setX(transaction, x);
        draggedBank->setY(transaction, y);

        m_presetManager.sanitizeBankClusterRelations(transaction);
      }
    }
  });

  addAction("undock-bank", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    const auto uuid = request->get("uuid");
    const auto x = request->get("x");
    const auto y = request->get("y");

    if(auto bank = m_presetManager.findBank(Uuid { uuid }))
    {
      if(auto attached = m_presetManager.findBank(bank->getAttachedToBankUuid()))
      {
        auto parentBankName = attached->getName(true);
        auto scope = presetManager.getUndoScope().startTransaction("Detached Bank '%0' from '%1'", bank->getName(true),
                                                                   parentBankName);
        auto transaction = scope->getTransaction();

        bank->attachBank(transaction, Uuid::none(), Bank::AttachmentDirection::none);
        bank->setX(transaction, x);
        bank->setY(transaction, y);
      }
    }
  });

  addAction("move-all-banks", [&](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto x = std::stof(request->get("x"));
    auto y = std::stof(request->get("y"));
    PresetManagerUseCases useCases(m_presetManager, m_settings);
    useCases.moveAllBanks(x, y);
  });

  addAction("sort-bank-numbers", [&](auto request) mutable {
    PresetManagerUseCases pmUseCases(m_presetManager, m_settings);
    pmUseCases.sortBankNumbers();
  });
}

BankActions::~BankActions()
= default;

bool BankActions::handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request)
{
  if(SectionAndActionManager::handleRequest(path, request))
    return true;

  if(path.find("/banks/download-bank/") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      Glib::ustring uuid = request->get("uuid");

      if(auto bank = m_presetManager.findBank(Uuid { uuid }))
      {
        MemoryOutStream stream;
        XmlWriter writer(stream);
        PresetBankSerializer serializer(bank, {});
        serializer.write(writer, VersionAttribute::get());
        auto disposition = "attachment; filename=\"" + bank->getName(true) + ".xml\"";
        httpRequest->respondComplete(SOUP_STATUS_OK, "text/xml", { { "Content-Disposition", disposition } },
                                     stream.exhaust());

        auto scope = UNDO::Scope::startTrashTransaction();
        auto transaction = scope->getTransaction();
        bank->setAttribute(transaction, "Name of Export File", "(via Browser)");
        auto adj = m_settings.getSetting<DateTimeAdjustment>();
        bank->setAttribute(transaction, "Date of Export File", TimeTools::getAdjustedIso(adj));
      }

      return true;
    }
  }

  if(path.find("/banks/download-preset/") == 0)
  {
    if(auto httpRequest = std::dynamic_pointer_cast<HTTPRequest>(request))
    {
      MemoryOutStream stream;
      XmlWriter writer(stream);
      Glib::ustring uuid = httpRequest->get("uuid");

      Preset* preset;
      auto ebAsPreset = std::make_unique<Preset>(&m_presetManager, *m_presetManager.getEditBuffer());

      if(uuid.empty())
      {
        preset = ebAsPreset.get();
      }
      else if(auto p = m_presetManager.findPreset(Uuid { uuid }))
      {
        preset = p;
      }
      else
      {
        nltools::Log::warning("Could not download Preset!");
        return false;
      }

      PresetSerializer serializer(preset);
      serializer.write(writer, VersionAttribute::get());
      auto disposition = "attachment; filename=\"" + preset->getName() + ".xml\"";
      httpRequest->respondComplete(SOUP_STATUS_OK, "text/xml", { { "Content-Disposition", disposition } },
                                   stream.exhaust());
      return true;
    }
  }

  return false;
}

void BankActions::insertBank(const std::shared_ptr<NetworkRequest>& request, size_t offset)
{
  auto anchorUuid = request->get("anchor");
  auto bankUuid = request->get("bank");

  if(auto preset = m_presetManager.findPreset(Uuid { anchorUuid }))
  {
    if(auto targetBank = dynamic_cast<Bank*>(preset->getParent()))
    {
      if(auto bank = m_presetManager.findBank(Uuid { bankUuid }))
      {
        if(bank != targetBank)
        {
          auto anchorPos = targetBank->getPresetPosition(Uuid { anchorUuid });
          BankUseCases useCase(targetBank, m_settings);
          useCase.insertBank(bank, anchorPos + offset);
        }
      }
    }
  }
}
