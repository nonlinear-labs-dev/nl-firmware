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

void XMLPresetLoader::sendToAE(EditBuffer *eb, C15Synth *synth)
{
  switch(eb->getType())
  {
    case SoundType::Single:
      synth->onSinglePresetMessage(AudioEngineProxy::createSingleEditBufferMessage(*eb));
      break;
    case SoundType::Split:
      synth->onSplitPresetMessage(AudioEngineProxy::createSplitEditBufferMessage(*eb));
      break;
    case SoundType::Layer:
      synth->onLayerPresetMessage(AudioEngineProxy::createLayerEditBufferMessage(*eb));
      break;
  }
}

void XMLPresetLoader::loadFirstPresetOfBank(Application *app, const std::string &fileName, C15Synth *synth)
{
  auto eb = app->getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  PresetManagerUseCases pmUseCases(*app->getPresetManager(), *app->getSettings());
  auto presetData = getSourceDir() + "/projects/epc/acceptance-tests/test-data";
  auto bank = pmUseCases.importBankFromPath(std::filesystem::directory_entry { presetData + "/xml-banks/" + fileName },
                                            [](auto) {});
  ebUseCases.load(bank->getPresetAt(0));

  sendToAE(eb, synth);
}
