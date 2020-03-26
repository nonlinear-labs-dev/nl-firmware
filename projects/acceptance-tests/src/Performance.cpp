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

  auto presetData = getSourceDir() + "/projects/acceptance-tests/test-data/performance-test.binary";
  auto file = Gio::File::create_for_path(presetData);
  auto transactionScope = undoScope.startTransaction("load");
  auto transaction = transactionScope->getTransaction();

  PresetManager pm(&updateDocMaster, true);
  EditBuffer editBuffer(&pm);
  Preset preset(&pm);
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
  }

  auto perfWithoutNote = synth->measurePerformance(1s);

  GIVEN("notes are played")
  {
    constexpr auto numNotes = 20;
    for(int i = 0; i < numNotes; i++)
      synth->simulateKeyDown(50 + i);

    auto perfWithNote = synth->measurePerformance(1s);

    WHEN("audio decays")
    {
      constexpr auto numNotes = 20;
      for(int i = 0; i < numNotes; i++)
        synth->simulateKeyUp(50 + i);

      auto perfInReleasePhase = synth->measurePerformance(1s);

      THEN("CPU usage is still the same after 10 sec")
      {
        synth->measurePerformance(60s);
        auto perfAfterReleasePhase = synth->measurePerformance(1s);
        nltools::Log::notify(perfWithoutNote, perfWithNote, perfInReleasePhase, perfAfterReleasePhase);
      }
    }
  }
}
