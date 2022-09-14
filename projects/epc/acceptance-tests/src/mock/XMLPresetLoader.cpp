#include "XMLPresetLoader.h"
#include "MockSettingsObject.h"
#include <AudioEngineOptions.h>
#include <http/UpdateDocumentMaster.h>
#include <libundo/undo/Scope.h>
#include <CompileTimeOptions.h>
#include <giomm.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <presets/Preset.h>
#include <synth/C15Synth.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <fstream>
#include <use-cases/PresetManagerUseCases.h>
#include <presets/Bank.h>
#include <Options.h>
#include <device-settings/Settings.h>
#include <sync/SyncMasterMockRoot.h>
#include <use-cases/EditBufferUseCases.h>
#include <presets/PresetParameter.h>
#include <testing/TestHelper.h>

void XMLPresetLoader::loadTestPreset(C15Synth* synth, const std::string& subDir, const std::string& uuid)
{
  UNDO::Scope undoScope(&SyncMasterMockRoot::get());

  auto presetData = getSourceDir() + "/projects/epc/acceptance-tests/test-data";

  if(!subDir.empty())
    presetData += "/" + subDir;

  auto file = Gio::File::create_for_path(presetData);
  auto transactionScope = undoScope.startTransaction("load");
  auto transaction = transactionScope->getTransaction();

  Options opt;
  MockSettingsObject settings("", &SyncMasterMockRoot::get());
  std::unique_ptr<AudioEngineProxy> proxy;
  PresetManager pm(&SyncMasterMockRoot::get(), true, opt, settings, proxy);
  auto& editBuffer = *pm.getEditBuffer();
  Preset preset(&pm);
  preset.setUuid(transaction, Uuid { uuid });
  preset.load(transaction, file);
  editBuffer.copyFrom(transaction, &preset);

  switch(editBuffer.getType())
  {
    case SoundType::Single:
      synth->onSinglePresetMessage(AudioEngineProxy::createSingleEditBufferMessage(editBuffer));
      break;

    case SoundType::Split:
      synth->onSplitPresetMessage(AudioEngineProxy::createSplitEditBufferMessage(editBuffer));
      break;

    case SoundType::Layer:
      synth->onLayerPresetMessage(AudioEngineProxy::createLayerEditBufferMessage(editBuffer));
      break;

    default:
      break;
  }
}

void XMLPresetLoader::loadTestPresetFromBank(C15Synth* synth, const std::string& subDir,
                                             const std::string& bankFileName, Settings& settings)
{
  auto presetData = getSourceDir() + "/projects/epc/acceptance-tests/test-data";

  if(!subDir.empty())
    presetData += "/" + subDir;

  presetData += "/" + bankFileName + ".xml";

  auto file = Gio::File::create_for_path(presetData);

  Options opt;
  auto oldPath = opt.getPresetManagerPath();
  opt.setPresetManagerPath(presetData);
  auto pproxy = std::make_unique<PlaycontrollerProxy>();

  std::unique_ptr<AudioEngineProxy> proxy;
  PresetManager pm(&SyncMasterMockRoot::get(), false, opt, settings, proxy);
  proxy = std::make_unique<AudioEngineProxy>(pm, settings, *pproxy);
  pm.init(proxy.get(), settings, Serializer::MockProgress);
  PresetManagerUseCases useCase(pm, settings);

  useCase.clear();
  useCase.importBankFromPath(std::filesystem::directory_entry { presetData }, Serializer::MockProgress);

  auto bank = pm.getBankAt(0);
  auto preset = bank->getPresetAt(0);

  nltools_assertAlways(bank != nullptr);
  nltools_assertAlways(preset != nullptr);

  EditBufferUseCases ebUseCase(*pm.getEditBuffer());
  ebUseCase.load(preset);
  
  auto& eb = *pm.getEditBuffer();
  auto p = eb.findParameterByID({ C15::PID::Out_Mix_A_Lvl, VoiceGroup::I });
  auto pp = preset->findParameterByID({ C15::PID::Out_Mix_A_Lvl, VoiceGroup::I }, true);

  switch(eb.getType())
  {
    case SoundType::Single:
      synth->onSinglePresetMessage(AudioEngineProxy::createSingleEditBufferMessage(eb));
      break;

    case SoundType::Split:
      synth->onSplitPresetMessage(AudioEngineProxy::createSplitEditBufferMessage(eb));
      break;

    case SoundType::Layer:
      synth->onLayerPresetMessage(AudioEngineProxy::createLayerEditBufferMessage(eb));
      break;

    default:
      break;
  }

  opt.setPresetManagerPath(oldPath);
}

void XMLPresetLoader::convertSoundTo(C15Synth* synth, Settings* settings, SoundType type)
{
  Options opt;
  std::unique_ptr<AudioEngineProxy> proxy;
  PresetManager pm(&SyncMasterMockRoot::get(), true, opt, *settings, proxy);
  auto& eb = *pm.getEditBuffer();
  EditBufferUseCases ebUseCase(eb);

  switch(type)
  {
    case SoundType::Single:
      ebUseCase.convertToSingle(VoiceGroup::I);
      break;
    case SoundType::Split:
      ebUseCase.convertToSplit(VoiceGroup::I);
      break;
    case SoundType::Layer:
      ebUseCase.convertToLayer(VoiceGroup::I);
      break;
  }

  switch(eb.getType())
  {
    case SoundType::Single:
      synth->onSinglePresetMessage(AudioEngineProxy::createSingleEditBufferMessage(eb));
      break;

    case SoundType::Split:
      synth->onSplitPresetMessage(AudioEngineProxy::createSplitEditBufferMessage(eb));
      break;

    case SoundType::Layer:
      synth->onLayerPresetMessage(AudioEngineProxy::createLayerEditBufferMessage(eb));
      break;
    default:
      break;
  }
}
