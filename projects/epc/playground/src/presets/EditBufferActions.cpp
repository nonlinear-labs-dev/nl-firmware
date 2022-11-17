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

#include <device-settings/RandomizeAmount.h>
#include <device-settings/Settings.h>

//NonMember helperFunctions pre:
IntrusiveList<EditBufferActions::tParameterPtr> getScaleParameters(EditBuffer& editBuffer);

EditBufferActions::EditBufferActions(UpdateDocumentContributor* parent, EditBuffer& editBuffer, AudioEngineProxy& aeProxy, Settings& settings)
    : SectionAndActionManager(parent, "/param-editor/")
{
  addAction("sync-audioengine", [&aeProxy](const std::shared_ptr<NetworkRequest>& request) mutable {
    aeProxy.sendEditBuffer();
  });

  addAction("restore", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    using namespace std::chrono;
    auto time = std::stoll(request->get("timestamp"));
    auto& scope = editBuffer.getUndoScope();

    if(auto transaction = scope.findTransactionAt(system_clock::time_point(system_clock::duration(time))))
    {
      auto currentTip = scope.getUndoTransaction();
      auto log = editBuffer.getLoadedPresetLog();
      log->freeze();
      scope.undoJump(UNDO::StringTools::buildString(reinterpret_cast<size_t>(transaction)));
      auto p = std::make_unique<Preset>(editBuffer.getParent(), editBuffer, Uuid::restored());
      scope.undoJump(UNDO::StringTools::buildString(reinterpret_cast<size_t>(currentTip)));

      EditBufferUseCases ebUseCases(editBuffer);
      std::stringstream nameBuilder;
      std::time_t tt = system_clock::to_time_t(system_clock::time_point(system_clock::duration(time)));
      std::tm tm = *std::localtime(&tt);
      nameBuilder << "Restore at " << std::put_time(&tm, "%H:%M:%S");
      ebUseCases.load(p.get(), nameBuilder.str());
      log->thaw();
    }
  });

  addAction("select-param", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    Glib::ustring id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.selectParameter(ParameterId(id), false);
  });

  addAction("set-param", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    auto value = std::stod(request->get("value"));
    EditBufferUseCases ebUseCases( editBuffer);
    ebUseCases.setParameter(ParameterId(id), value);
  });

  addAction("set-splits", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    auto value = std::stod(request->get("value"));
    auto otherValue = std::stod(request->get("other-value"));
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.setSplits(ParameterId(id), value, otherValue);
  });

  addAction("set-mod-amount", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto amount = std::stod(request->get("amount"));
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.setModulationAmount(amount, ParameterId { id });
  });

  addAction("set-mod-src", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto src = std::stoi(request->get("source"));
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.setModulationSource(static_cast<MacroControls>(src), ParameterId { id });
  });

  addAction("reset", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    EditBufferUseCases useCase(editBuffer);
    useCase.initSound(Defaults::UserDefault);
  });

  addAction("rename-mc", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    auto newName = request->get("new-name");
    EditBufferUseCases useCase { editBuffer };
    if(auto mcUseCase = useCase.getMCUseCase(ParameterId { id }))
    {
      mcUseCase->setName(newName);
    }
  });

  addAction("reset-scale", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    EditBufferUseCases useCase { editBuffer };
    useCase.resetScaleGroup();
  });

  addAction("set-macrocontrol-info", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    auto info = request->get("info");
    EditBufferUseCases useCase { editBuffer };
    if(auto mcUseCase = useCase.getMCUseCase(ParameterId { id }))
    {
      mcUseCase->setInfo(info);
    }
  });

  addAction("set-ribbon-touch-behaviour", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    Glib::ustring mode = request->get("mode");

    if(auto param = dynamic_cast<RibbonParameter*>(editBuffer.findParameterByID(ParameterId(id))))
    {
      RibbonParameterUseCases useCase(param);
      useCase.setTouchBehaviour(mode);
    }
  });

  addAction("set-ribbon-return-mode", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    auto mode = request->get("mode");

    if(auto param = dynamic_cast<RibbonParameter*>(editBuffer.findParameterByID(ParameterId(id))))
    {
      RibbonParameterUseCases useCase(param);
      useCase.setReturnMode(mode);
    }
  });

  addAction("set-ribbon-behaviour", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
     auto id = request->get("id");
     auto mode = request->get("mode");

     if(auto param = dynamic_cast<RibbonParameter*>(editBuffer.findParameterByID(ParameterId(id))))
     {
       RibbonParameterUseCases useCases(param);
       useCases.setTouchBehaviour(to<RibbonTouchBehaviour>(mode.uppercase()));
     }
  });

  addAction("set-pedal-mode", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    Glib::ustring mode = request->get("mode");

    if(auto param = dynamic_cast<PedalParameter*>(editBuffer.findParameterByID(ParameterId(id))))
    {
      PedalParameterUseCases useCase(param);
      useCase.setPedalMode(mode);
    }
  });

  addAction("reset-modulation", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    if(auto mcUseCase = ebUseCases.getMCUseCase(ParameterId { id }))
    {
      mcUseCase->resetConnectionsToTargets();
    }
  });

  addAction("randomize-sound", [&editBuffer, &s = settings](const std::shared_ptr<NetworkRequest>&) mutable {
    auto amount = s.getSetting<RandomizeAmount>()->get();
    EditBufferUseCases useCase { editBuffer };
    useCase.randomize(amount);
  });

  addAction("init-sound", [&editBuffer](const std::shared_ptr<NetworkRequest>&) mutable {
    EditBufferUseCases useCase { editBuffer };
    useCase.initSound(Defaults::UserDefault);
  });

  addAction("init-part", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    EditBufferUseCases useCase { editBuffer };
    useCase.initPart(to<VoiceGroup>(request->get("part")), Defaults::UserDefault);
  });

  addAction("rename-part", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    auto name = request->get("name");
    SoundUseCases soundUseCases { &editBuffer, editBuffer.getParent() };
    soundUseCases.renamePart(vg, name);
  });

  addAction("randomize-part", [&editBuffer, &s = settings](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto amount = s.getSetting<RandomizeAmount>()->get();
    EditBufferUseCases useCase { editBuffer };
    useCase.randomizePart(to<VoiceGroup>(request->get("part")), amount);
  });

  addAction("mute", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    EditBufferUseCases useCase(editBuffer);
    useCase.mutePart(vg);
  });

  addAction("unmute", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.unmutePart(vg);
  });

  addAction("mute-part-unmute-other", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto vg = to<VoiceGroup>(request->get("part"));
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.mutePartUnmuteOtherPart(vg);
  });

  addAction("set-modamount-and-value", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = request->get("id");
    auto modAmount = std::stod(request->get("mod-amount"));
    auto value = std::stod(request->get("value"));
    auto paramId = ParameterId(id);
    EditBufferUseCases useCase(editBuffer);
    useCase.setModAmountAndValue(paramId, modAmount, value);
  });

  addAction("set-modulation-limit", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto id = ParameterId { request->get("id") };
    auto newAmt = std::stod(request->get("mod-amt"));
    auto newParamVal = std::stod(request->get("param-val"));
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.setModulationLimit(id, newAmt, newParamVal);
  });

  addAction("unlock-all-groups", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.unlockAllGroups();
  });

  addAction("lock-all-groups", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.lockAllGroups();
  });

  addAction("toggle-group-lock", [&editBuffer](const std::shared_ptr<NetworkRequest>& request) mutable {
    auto groupId = request->get("group");

    EditBufferUseCases ebUseCases(editBuffer);
    ebUseCases.toggleLock(groupId);
  });

  addAction("recall-current-from-preset", [&editBuffer](auto request) {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    if(auto paramUseCase = ebUseCases.getUseCase(ParameterId { id }))
    {
      paramUseCase->recallParameterFromPreset();
    }
  });

  addAction("recall-mc-for-current-mod-param", [&editBuffer](auto request) {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    if(auto modUseCase = ebUseCases.getModParamUseCase(ParameterId { id }))
      modUseCase->recallMCPos();
  });

  addAction("recall-mc-amount-for-current-mod-param", [&editBuffer](auto request) {
    auto id = request->get("id");
    EditBufferUseCases ebUseCases(editBuffer);
    if(auto modUseCase = ebUseCases.getModParamUseCase(ParameterId { id }))
      modUseCase->recallMCAmount();
  });

  addAction("convert-to-single", [&editBuffer](auto request) {
    auto voiceGroup = to<VoiceGroup>(request->get("voice-group"));
    EditBufferUseCases useCase { editBuffer };
    useCase.convertToSingle(voiceGroup);
    //TODO move functionality someplace else -> hwui subscribe to eb->onConverted?
    //Application::get().getHWUI()->setCurrentVoiceGroupAndUpdateParameterSelection(transaction, VoiceGroup::I);
  });

  addAction("convert-to-split", [&editBuffer](auto request) {
    EditBufferUseCases useCase { editBuffer };
    useCase.convertToSplit(VoiceGroup::I); // TODO pass current VG from WebUI
  });

  addAction("convert-to-layer", [&editBuffer](auto request) {
    EditBufferUseCases useCase { editBuffer };
    useCase.convertToLayer(VoiceGroup::II); //TODO pass current VG from WebUI
  });

  addAction("load-selected-preset-part-into-editbuffer-part", [&editBuffer](auto request) {
    auto presetPart = to<VoiceGroup>(request->get("preset-part"));
    auto editbufferPartPart = to<VoiceGroup>(request->get("editbuffer-part"));

    EditBufferUseCases ebUseCases(editBuffer);
    if(auto selectedPreset = editBuffer.getParent()->getSelectedPreset())
    {
      ebUseCases.loadToPart(selectedPreset, presetPart, editbufferPartPart);
    }
  });

  addAction("load-preset-part-into-editbuffer-part", [&editBuffer](auto request) {
    auto presetPart = to<VoiceGroup>(request->get("preset-part"));
    auto loadTo = to<VoiceGroup>(request->get("editbuffer-part"));
    auto presetUUID = request->get("preset-uuid");
    auto pm = editBuffer.getParent();

    EditBufferUseCases ebUseCases(editBuffer);
    if(auto presetToLoad = pm->findPreset(Uuid { presetUUID }))
    {
      ebUseCases.loadToPart(presetToLoad, presetPart, loadTo);
    }
  });

  addAction("default-param", [&editBuffer](auto request) {
    auto id = ParameterId(request->get("id"));
    if(auto p = editBuffer.findParameterByID(id))
    {
      ParameterUseCases useCase(p);
      useCase.toggleLoadDefault();
    }
  });
}