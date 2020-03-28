#include <catch.hpp>
#include <synth/C15Synth.h>
#include <presets/Preset.h>
#include <AudioEngineOptions.h>
#include <CompileTimeOptions.h>
#include <libundo/undo/Scope.h>
#include <http/UpdateDocumentMaster.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <giomm.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <fstream>

class UpdateDocumentMasterMock : public UpdateDocumentMaster
{
 public:
  void writeDocument(Writer &, tUpdateID) const override
  {
  }
};

SCENARIO("CPU Usage is constant")
{
  using namespace std::chrono_literals;

  int argc = 0;
  char **argv = nullptr;
  auto options = std::make_unique<AudioEngineOptions>(argc, argv);
  auto synth = std::make_unique<C15Synth>(options.get());

  UpdateDocumentMasterMock updateDocMaster;
  UNDO::Scope undoScope(&updateDocMaster);

  auto presetData = getSourceDir() + "/projects/acceptance-tests/test-data";
  auto file = Gio::File::create_for_path(presetData);
  auto transactionScope = undoScope.startTransaction("load");
  auto transaction = transactionScope->getTransaction();

  PresetManager pm(&updateDocMaster, true);
  EditBuffer editBuffer(&pm);
  Preset preset(&pm);
  preset.setUuid(transaction, "3c830098-c38b-461c-aa55-2548960c9064");
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

  auto dump = [](auto &data, auto fileName) {
    std::ofstream file(fileName);
    auto &audioBlock = std::get<0>(data);
    file.write(reinterpret_cast<const char *>(audioBlock.data()),
               static_cast<std::streamsize>(audioBlock.size() * sizeof(SampleFrame)));
  };

  auto withoutNote = synth->measurePerformance(1s);
  dump(withoutNote, "/tmp/withoutNote.raw");

  GIVEN("notes are played")
  {
    synth->simulateKeyDown(50);
    auto withNote = synth->measurePerformance(4s);
    dump(withNote, "/tmp/withNote.raw");

    WHEN("audio decays")
    {
      synth->simulateKeyUp(50);

      THEN("CPU usage is still the same after some sec")
      {
        synth->measurePerformance(30s);
        auto longAfterRelease = synth->measurePerformance(1s);
        dump(longAfterRelease, "/tmp/afterRelease.raw");
        nltools::Log::notify(std::get<1>(withoutNote), std::get<1>(longAfterRelease));
        nltools::Log::notify("samples rendered:", synth->getRenderedSamples());
      }
    }
  }
}
