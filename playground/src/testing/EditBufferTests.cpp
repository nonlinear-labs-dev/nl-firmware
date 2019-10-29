#include <third-party/include/catch.hpp>

#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <parameters/PedalParameter.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <nltools/Types.h>
#include <presets/Bank.h>
#include <presets/Preset.h>

inline auto getEditBuffer() -> EditBuffer*
{
  return Application::get().getPresetManager()->getEditBuffer();
}

inline auto createTestScope() -> std::unique_ptr<UNDO::TransactionCreationScope>
{
  return std::move(Application::get().getPresetManager()->getUndoScope().startTestTransaction());
}

TEST_CASE("EditBuffer Initialized")
{
  auto eb = getEditBuffer();
  REQUIRE(eb != nullptr);
}

TEST_CASE("editbuffer can be converted")
{
  auto editBuffer = getEditBuffer();

  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
  }

  REQUIRE(editBuffer->getType() == SoundType::Single);

  SECTION("convert single editbuffer to layer")
  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Layer);
  }

  SECTION("convert single editbuffer to split")
  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Split);
  }

  SECTION("convert single editbuffer to split and undo")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
    }

    REQUIRE(editBuffer->getType() == SoundType::Split);

    editBuffer->getParent()->getUndoScope().undo();

    REQUIRE(editBuffer->getType() == SoundType::Single);
  }
}

TEST_CASE("master groups initialization tests")
{
  auto editBuffer = getEditBuffer();

  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
  }

  auto collectScaleParameterValues = [editBuffer] {
    std::vector<double> ret;
    if(auto g = editBuffer->getGlobalParameterGroupByID("Scale"))
      for(auto p : g->getParameters())
        ret.emplace_back(p->getControlPositionValue());
    return ret;
  };

  const auto globalMasterVolume = editBuffer->findGlobalParameterByID(247)->getControlPositionValue();
  const auto globalMasterTune = editBuffer->findGlobalParameterByID(248)->getControlPositionValue();
  const auto globalScaleValues = collectScaleParameterValues();

  auto compareVoiceGroupMasterSameAsGlobalMaster = [globalMasterVolume, globalMasterTune, editBuffer]() {
    for(auto& vg : { VoiceGroup::I, VoiceGroup::II })
    {
      auto vgMasterVolume = editBuffer->findParameterByID(10002, vg)->getControlPositionValue();
      auto vgMasterTune = editBuffer->findParameterByID(10003, vg)->getControlPositionValue();

      REQUIRE(vgMasterVolume == globalMasterVolume);
      REQUIRE(vgMasterTune == globalMasterTune);
    }
  };

  auto globalMasterSetToDefault = [editBuffer]() {
    auto globalMasterVolume = editBuffer->findGlobalParameterByID(247)->getControlPositionValue();
    auto globalMasterTune = editBuffer->findGlobalParameterByID(248)->getControlPositionValue();

    REQUIRE(globalMasterVolume == 0.5);
    REQUIRE(globalMasterTune == 0);
  };

  auto scaleUntouched = [editBuffer, collectScaleParameterValues](const std::vector<double>& scaleValues) {
    auto currentScaleValues = collectScaleParameterValues();

    REQUIRE(scaleValues.size() == currentScaleValues.size());
    REQUIRE(scaleValues == currentScaleValues);
  };

  SECTION("split global master copied to voice groups master and set to default")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
    }

    compareVoiceGroupMasterSameAsGlobalMaster();
    globalMasterSetToDefault();
    scaleUntouched(globalScaleValues);
  }

  SECTION("layer global master copied to voice groups master and set to default")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer);
    }

    compareVoiceGroupMasterSameAsGlobalMaster();
    globalMasterSetToDefault();
    scaleUntouched(globalScaleValues);
  }
}

TEST_CASE("convert back and forth")
{
  auto editBuffer = getEditBuffer();
  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
  }

  auto hash = editBuffer->getHash();

  SECTION("split and back")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
    }

    auto splitHash = editBuffer->getHash();
    REQUIRE(splitHash != hash);

    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    }

    auto singleHash = editBuffer->getHash();
    REQUIRE(singleHash != splitHash);
    REQUIRE(singleHash == hash);
  }

  SECTION("split and undo")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
    }

    auto splitHash = editBuffer->getHash();
    REQUIRE(splitHash != hash);

    editBuffer->getParent()->getUndoScope().undo();

    auto singleHash = editBuffer->getHash();
    REQUIRE(singleHash != splitHash);
    REQUIRE(singleHash == hash);
  }
}

TEST_CASE("poly groups initialization")
{
  auto editBuffer = getEditBuffer();

  const auto unisonVoices = editBuffer->findParameterByID(249, VoiceGroup::I)->getControlPositionValue();
  const auto unisonDetune = editBuffer->findParameterByID(250, VoiceGroup::I)->getControlPositionValue();
  const auto unisonPhase = editBuffer->findParameterByID(252, VoiceGroup::I)->getControlPositionValue();
  const auto unisonPan = editBuffer->findParameterByID(253, VoiceGroup::I)->getControlPositionValue();

  const auto monoEnable = editBuffer->findParameterByID(12345, VoiceGroup::I)->getControlPositionValue();
  const auto monoPrio = editBuffer->findParameterByID(12346, VoiceGroup::I)->getControlPositionValue();
  const auto monoLegato = editBuffer->findParameterByID(12347, VoiceGroup::I)->getControlPositionValue();
  const auto monoGlide = editBuffer->findParameterByID(12348, VoiceGroup::I)->getControlPositionValue();

  SECTION("poly groups I, II and original are equal")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);
    }

    for(auto& vg : { VoiceGroup::I, VoiceGroup::II })
    {
      const auto vgVoices = editBuffer->findParameterByID(249, vg)->getControlPositionValue();
      const auto vgDetune = editBuffer->findParameterByID(250, vg)->getControlPositionValue();
      const auto vgPhase = editBuffer->findParameterByID(252, vg)->getControlPositionValue();
      const auto vgPan = editBuffer->findParameterByID(253, vg)->getControlPositionValue();

      const auto vgEnable = editBuffer->findParameterByID(12345, vg)->getControlPositionValue();
      const auto vgPrio = editBuffer->findParameterByID(12346, vg)->getControlPositionValue();
      const auto vgLegato = editBuffer->findParameterByID(12347, vg)->getControlPositionValue();
      const auto vgGlide = editBuffer->findParameterByID(12348, vg)->getControlPositionValue();

      REQUIRE(unisonVoices == vgVoices);
      REQUIRE(unisonDetune == vgDetune);
      REQUIRE(unisonPhase == vgPhase);
      REQUIRE(unisonPan == vgPan);

      REQUIRE(monoEnable == vgEnable);
      REQUIRE(monoPrio == vgPrio);
      REQUIRE(monoLegato == vgLegato);
      REQUIRE(monoGlide == vgGlide);
    }
  }

  SECTION("poly groups I, II and original same")
  {
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
    }

    for(auto& vg : { VoiceGroup::I, VoiceGroup::II })
    {
      const auto vgVoices = editBuffer->findParameterByID(249, vg)->getControlPositionValue();
      const auto vgDetune = editBuffer->findParameterByID(250, vg)->getControlPositionValue();
      const auto vgPhase = editBuffer->findParameterByID(252, vg)->getControlPositionValue();
      const auto vgPan = editBuffer->findParameterByID(253, vg)->getControlPositionValue();

      const auto vgEnable = editBuffer->findParameterByID(12345, vg)->getControlPositionValue();
      const auto vgPrio = editBuffer->findParameterByID(12346, vg)->getControlPositionValue();
      const auto vgLegato = editBuffer->findParameterByID(12347, vg)->getControlPositionValue();
      const auto vgGlide = editBuffer->findParameterByID(12348, vg)->getControlPositionValue();

      REQUIRE(unisonVoices == vgVoices);
      REQUIRE(unisonDetune == vgDetune);
      REQUIRE(unisonPhase == vgPhase);
      REQUIRE(unisonPan == vgPan);

      REQUIRE(monoEnable == vgEnable);
      REQUIRE(monoPrio == vgPrio);
      REQUIRE(monoLegato == vgLegato);
      REQUIRE(monoGlide == vgGlide);
    }
  }
}

TEST_CASE("Split to Single Conversion")
{
  auto editBuffer = getEditBuffer();

  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
  }

  REQUIRE(editBuffer->getType() == SoundType::Split);

  SECTION("add voice-group master params and set init global master")
  {
    auto vgmasterI = editBuffer->findParameterByID(10002, VoiceGroup::I);
    auto vgmasterII = editBuffer->findParameterByID(10002, VoiceGroup::II);
    auto vgtuneI = editBuffer->findParameterByID(10003, VoiceGroup::I);
    auto vgtuneII = editBuffer->findParameterByID(10003, VoiceGroup::II);

    //Update vg master parameters
    {
      auto scope = createTestScope();
      vgmasterI->setCPFromHwui(scope->getTransaction(), 0.5);
      vgmasterII->setCPFromHwui(scope->getTransaction(), 0.2);

      vgtuneI->setCPFromHwui(scope->getTransaction(), 0.5);
      vgtuneII->setCPFromHwui(scope->getTransaction(), 0.2);
    }

    //Convert sound to single
    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    }

    auto globalMasterVolume = editBuffer->findGlobalParameterByID(247);
    auto globalMasterTune = editBuffer->findGlobalParameterByID(248);

    REQUIRE(globalMasterVolume->getControlPositionValue() == 0.7);
    REQUIRE(globalMasterTune->getControlPositionValue() == 0.7);
  }
}

TEST_CASE("Load Presets Complete")
{

  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  Bank* bank = nullptr;

  Uuid singleuuid;
  Uuid splituuid;
  Uuid layeruuid;

  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    bank = pm->addBank(scope->getTransaction());
    auto singlePreset = bank->appendPreset(scope->getTransaction(), std::make_unique<Preset>(bank, *editBuffer));
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
    auto splitPreset = bank->appendPreset(scope->getTransaction(), std::make_unique<Preset>(bank, *editBuffer));
    editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);
    auto layerPreset = bank->appendPreset(scope->getTransaction(), std::make_unique<Preset>(bank, *editBuffer));

    REQUIRE(singlePreset != nullptr);
    REQUIRE(layerPreset != nullptr);
    REQUIRE(splitPreset != nullptr);

    singlePreset->setName(scope->getTransaction(), "Single Preset");
    splitPreset->setName(scope->getTransaction(), "Split Preset");
    layerPreset->setName(scope->getTransaction(), "Layer Preset");

    REQUIRE(singlePreset->getType() == SoundType::Single);
    REQUIRE(splitPreset->getType() == SoundType::Split);
    REQUIRE(layerPreset->getType() == SoundType::Layer);

    singleuuid = singlePreset->getUuid();
    splituuid = splitPreset->getUuid();
    layeruuid = layerPreset->getUuid();
  }

  SECTION("Load single preset")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), pm->findPreset(singleuuid));

    REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == singleuuid);
    REQUIRE(editBuffer->getType() == SoundType::Single);
  }

  SECTION("Load layer preset")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), pm->findPreset(layeruuid));

    REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == layeruuid);
    REQUIRE(editBuffer->getType() == SoundType::Layer);
  }

  SECTION("Load split preset")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), pm->findPreset(splituuid));

    REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == splituuid);
    REQUIRE(editBuffer->getType() == SoundType::Split);
  }

  //cleanup
  {
    auto scope = createTestScope();
    pm->deleteBank(scope->getTransaction(), bank->getUuid());
    REQUIRE(pm->findBank(bank->getUuid()) == nullptr);
    REQUIRE(pm->findPreset(singleuuid) == nullptr);
    REQUIRE(pm->findPreset(layeruuid) == nullptr);
    REQUIRE(pm->findPreset(splituuid) == nullptr);
  }
}