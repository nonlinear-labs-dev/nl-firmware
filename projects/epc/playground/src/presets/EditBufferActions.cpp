#include <presets/EditBufferActions.h>
#include <presets/Preset.h>
#include <presets/LoadedPresetLog.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>

#include <parameters/RibbonParameter.h>
#include <parameters/PedalParameter.h>

#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

#include <boost/algorithm/string.hpp>

#include <http/HTTPRequest.h>
#include <http/UndoScope.h>
#include <http/SoupOutStream.h>
#include <iomanip>

#include <use-cases/ParameterUseCases.h>
#include <use-cases/RibbonParameterUseCases.h>
#include <use-cases/PedalParameterUseCases.h>
#include <use-cases/EditBufferUseCases.h>
#include <use-cases/SoundUseCases.h>

//NonMember helperFunctions pre:
IntrusiveList<EditBufferActions::tParameterPtr> getScaleParameters(EditBuffer& editBuffer);

EditBufferActions::EditBufferActions(EditBuffer& editBuffer, AudioEngineProxy& aeProxy)
    : super("/presets/param-editor/")
{
  auto aeProxyPtr = &aeProxy;
  auto editBufferPtr = &editBuffer;

  addAction("sync-audioengine", [aeProxyPtr](std::shared_ptr<NetworkRequest> request) mutable {
    aeProxyPtr->sendEditBuffer();
  });

  addAction("restore", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    using namespace std::chrono;
    auto time = std::stoll(request->get("timestamp"));
    auto& scope = editBufferPtr->getUndoScope();

    if(auto transaction = scope.findTransactionAt(system_clock::time_point(system_clock::duration(time))))
    {
      auto currentTip = scope.getUndoTransaction();
      auto log = editBufferPtr->getLoadedPresetLog();
      log->freeze();
      scope.undoJump(UNDO::StringTools::buildString(reinterpret_cast<size_t>(transaction)));
      auto p = std::make_unique<Preset>(editBufferPtr->getParent(), *editBufferPtr, Uuid::restored());
      scope.undoJump(UNDO::StringTools::buildString(reinterpret_cast<size_t>(currentTip)));
      EditBufferUseCases ebUseCases(*editBufferPtr);
      std::stringstream nameBuilder;
      std::time_t tt = system_clock::to_time_t(system_clock::time_point(system_clock::duration(time)));
      std::tm tm = *std::localtime(&tt);
      nameBuilder << "Restore at " << std::put_time(&tm, "%H:%M:%S");
      ebUseCases.undoableLoad(p.get(), nameBuilder.str());
      log->thaw();
    }
  });

  addAction("select-param", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    Glib::ustring id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.selectParameter(ParameterId(id), false);
  });

  addAction("set-param", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    auto value = std::stod(request->get("value"));
    EditBufferUseCases ebUseCases( *editBufferPtr);
    ebUseCases.setParameter(ParameterId(id), value);
  });

  addAction("set-splits", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    auto value = std::stod(request->get("value"));
    auto otherValue = std::stod(request->get("other-value"));
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.setSplits(ParameterId(id), value, otherValue);
  });

  addAction("set-mod-amount", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    auto amount = std::stod(request->get("amount"));
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.setModulationAmount(amount, ParameterId { id });
  });

  addAction("set-mod-src", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    auto src = std::stoi(request->get("source"));
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.setModulationSource(static_cast<MacroControls>(src), ParameterId { id });
  });

  addAction("reset", [editBufferPtr](std::shared_ptr<NetworkRequest>) mutable {
    SoundUseCases useCase { editBufferPtr, editBufferPtr->getParent() };
    useCase.initSound();
  });

  addAction("rename-mc", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    auto newName = request->get("new-name");
    EditBufferUseCases useCase { *editBufferPtr };
    if(auto mcUseCase = useCase.getMCUseCase(ParameterId { id }))
    {
      mcUseCase->setName(newName);
    }
  });

  addAction("reset-scale", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    EditBufferUseCases useCase { *editBufferPtr };
    useCase.resetScaleGroup();
  });

  addAction("set-macrocontrol-info", [editBufferPtr](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    auto info = request->get("info");
    EditBufferUseCases useCase { *editBufferPtr };
    if(auto mcUseCase = useCase.getMCUseCase(ParameterId { id }))
    {
      mcUseCase->setInfo(info);
    }
  });

  addAction("set-ribbon-touch-behaviour", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    Glib::ustring mode = request->get("mode");

    if(auto param = dynamic_cast<RibbonParameter*>(editBufferPtr->findParameterByID(ParameterId(id))))
    {
      RibbonParameterUseCases useCase(param);
      useCase.setTouchBehaviour(mode);
    }
  });

  addAction("set-ribbon-return-mode", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    auto mode = request->get("mode");

    if(auto param = dynamic_cast<RibbonParameter*>(editBufferPtr->findParameterByID(ParameterId(id))))
    {
      RibbonParameterUseCases useCase(param);
      useCase.setReturnMode(mode);
    }
  });

  addAction("set-pedal-mode", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    Glib::ustring mode = request->get("mode");

    if(auto param = dynamic_cast<PedalParameter*>(editBufferPtr->findParameterByID(ParameterId(id))))
    {
      PedalParameterUseCases useCase(param);
      useCase.setPedalMode(mode);
    }
  });

  addAction("reset-modulation", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    if(auto mcUseCase = ebUseCases.getMCUseCase(ParameterId { id }))
    {
      mcUseCase->resetConnectionsToTargets();
    }
  });

  addAction("randomize-sound", [=](const std::shared_ptr<NetworkRequest>&) mutable {
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.randomizeSound();
  });

  addAction("init-sound", [=](const std::shared_ptr<NetworkRequest>&) mutable {
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.initSound();
  });

  addAction("init-part", [=](const std::shared_ptr<NetworkRequest>& request) mutable {
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.initPart(to<VoiceGroup>(request->get("part")));
  });

  addAction("rename-part", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    auto name = request->get("name");
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.renamePart(vg, name);
  });

  addAction("randomize-part", [=](std::shared_ptr<NetworkRequest> request) mutable {
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.randomizePart(to<VoiceGroup>(request->get("part")));
  });

  addAction("mute", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    EditBufferUseCases useCase(*editBufferPtr);
    useCase.mutePart(vg);
  });

  addAction("unmute", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.unmutePart(vg);
  });

  addAction("mute-part-unmute-other", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.mutePartUnmuteOtherPart(vg);
  });

  addAction("set-modamount-and-value", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = request->get("id");
    auto modAmount = std::stod(request->get("mod-amount"));
    auto value = std::stod(request->get("value"));
    auto paramId = ParameterId(id);
    EditBufferUseCases useCase(*editBufferPtr);
    useCase.setModAmountAndValue(paramId, modAmount, value);
  });

  addAction("set-modulation-limit", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto id = ParameterId { request->get("id") };
    auto newAmt = std::stod(request->get("mod-amt"));
    auto newParamVal = std::stod(request->get("param-val"));
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.setModulationLimit(id, newAmt, newParamVal);
  });

  addAction("unlock-all-groups", [=](std::shared_ptr<NetworkRequest> request) mutable {
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.unlockAllGroups();
  });

  addAction("lock-all-groups", [=](std::shared_ptr<NetworkRequest> request) mutable {
    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.lockAllGroups();
  });

  addAction("toggle-group-lock", [=](std::shared_ptr<NetworkRequest> request) mutable {
    auto groupId = request->get("group");

    EditBufferUseCases ebUseCases(*editBufferPtr);
    ebUseCases.toggleLock(groupId);
  });

  addAction("recall-current-from-preset", [=](auto request) {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    if(auto paramUseCase = ebUseCases.getUseCase(ParameterId { id }))
    {
      paramUseCase->recallParameterFromPreset();
    }
  });

  addAction("recall-mc-for-current-mod-param", [=](auto request) {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    if(auto modUseCase = ebUseCases.getModParamUseCase(ParameterId { id }))
      modUseCase->recallMCPos();
  });

  addAction("recall-mc-amount-for-current-mod-param", [=](auto request) {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(*editBufferPtr);
    if(auto modUseCase = ebUseCases.getModParamUseCase(ParameterId { id }))
      modUseCase->recallMCAmount();
  });

  addAction("convert-to-single", [=](auto request) {
    auto voiceGroup = to<VoiceGroup>(request->get("voice-group"));
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.convertToSingle(voiceGroup);
    //TODO move functionality someplace else -> hwui subscribe to eb->onConverted?
    //Application::get().getHWUI()->setCurrentVoiceGroupAndUpdateParameterSelection(transaction, VoiceGroup::I);
  });

  addAction("convert-to-split", [=](auto request) {
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.convertToSplit();
  });

  addAction("convert-to-layer", [=](auto request) {
    SoundUseCases soundUseCases { editBufferPtr, editBufferPtr->getParent() };
    soundUseCases.convertToLayer();
  });

  addAction("load-selected-preset-part-into-editbuffer-part", [=](auto request) {
    auto presetPart = to<VoiceGroup>(request->get("preset-part"));
    auto editbufferPartPart = to<VoiceGroup>(request->get("editbuffer-part"));

    EditBufferUseCases ebUseCases(*editBufferPtr);
    if(auto selectedPreset = editBufferPtr->getParent()->getSelectedPreset())
    {
      ebUseCases.undoableLoadToPart(selectedPreset, presetPart, editbufferPartPart);
    }
  });

  addAction("load-preset-part-into-editbuffer-part", [=](auto request) {
    auto presetPart = to<VoiceGroup>(request->get("preset-part"));
    auto loadTo = to<VoiceGroup>(request->get("editbuffer-part"));
    auto presetUUID = request->get("preset-uuid");
    auto pm = editBufferPtr->getParent();

    EditBufferUseCases ebUseCases(*editBufferPtr);
    if(auto presetToLoad = pm->findPreset(Uuid { presetUUID }))
    {
      ebUseCases.undoableLoadToPart(presetToLoad, presetPart, loadTo);
    }
  });

  //TODO move this into HWUI actions?

  addAction("default-param", [=](auto request) {
    auto id = ParameterId(request->get("id"));
    if(auto p = editBufferPtr->findParameterByID(id))
    {
      ParameterUseCases useCase(p);
      useCase.toggleLoadDefault();
    }
  });
}
