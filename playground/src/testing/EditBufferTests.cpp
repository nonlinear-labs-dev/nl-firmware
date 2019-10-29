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

Preset* createSinglePreset(UNDO::Transaction* transaction)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  editBuffer->undoableConvertToSingle(transaction, VoiceGroup::I);
  auto bank = pm->addBank(transaction);
  auto preset = bank->appendPreset(transaction, std::make_unique<Preset>(bank, *editBuffer));
  preset->setName(transaction, "Single Preset");
  return preset;
}

Preset* createSplitPreset(UNDO::Transaction* transaction)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  editBuffer->undoableConvertToSingle(transaction, VoiceGroup::I);

  editBuffer->undoableConvertToDual(transaction, SoundType::Split, VoiceGroup::I);

  auto bank = pm->addBank(transaction);
  auto preset = bank->appendPreset(transaction, std::make_unique<Preset>(bank, *editBuffer));
  preset->setName(transaction, "Split Preset");
  return preset;
}

Preset* createLayerPreset(UNDO::Transaction* transaction)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  editBuffer->undoableConvertToDual(transaction, SoundType::Layer, VoiceGroup::I);
  auto bank = pm->addBank(transaction);
  auto preset = bank->appendPreset(transaction, std::make_unique<Preset>(bank, *editBuffer));
  preset->setName(transaction, "Layer Preset");
  return preset;
}

void removeBankOfPreset(UNDO::Transaction* transaction, Preset* presetToDelete)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();
  auto bank = dynamic_cast<Bank*>(presetToDelete->getParent());
  if(bank)
    pm->deleteBank(transaction, bank->getUuid());
}

class MockPresetStorage
{
 public:
  MockPresetStorage()
  {
    auto scope = createTestScope();
    auto transaction = scope->getTransaction();
    m_layer = createLayerPreset(transaction);
    m_split = createSplitPreset(transaction);
    m_single = createSinglePreset(transaction);
  }
  ~MockPresetStorage()
  {
    auto scope = createTestScope();
    auto transaction = scope->getTransaction();
    removeBankOfPreset(transaction, m_single);
    removeBankOfPreset(transaction, m_split);
    removeBankOfPreset(transaction, m_layer);
  }
  Preset* getSinglePreset()
  {
    return m_single;
  }
  Preset* getSplitPreset()
  {
    return m_split;
  }
  Preset* getLayerPreset()
  {
    return m_layer;
  }

 private:
  Preset* m_single;
  Preset* m_split;
  Preset* m_layer;
};

TEST_CASE("Preset Mock Storage")
{
  MockPresetStorage presets;
  REQUIRE(presets.getSinglePreset() != nullptr);
  REQUIRE(presets.getSinglePreset()->getType() == SoundType::Single);
  REQUIRE(presets.getSplitPreset() != nullptr);
  REQUIRE(presets.getSplitPreset()->getType() == SoundType::Split);
  REQUIRE(presets.getLayerPreset() != nullptr);
  REQUIRE(presets.getLayerPreset()->getType() == SoundType::Layer);
}

TEST_CASE("Load Presets Complete")
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();
  auto numBanks = pm->getNumBanks();

  {
    MockPresetStorage presets;

    SECTION("Load single preset")
    {
      auto scope = createTestScope();
      auto singlePreset = presets.getSinglePreset();
      editBuffer->undoableLoad(scope->getTransaction(), singlePreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == singlePreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Single);
    }

    SECTION("Load layer preset")
    {
      auto scope = createTestScope();
      auto layerPreset = presets.getLayerPreset();
      editBuffer->undoableLoad(scope->getTransaction(), layerPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == layerPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Layer);
    }

    SECTION("Load split preset")
    {
      auto scope = createTestScope();
      auto splitPreset = presets.getSplitPreset();
      editBuffer->undoableLoad(scope->getTransaction(), splitPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == splitPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Split);
    }
  }

  REQUIRE(numBanks == pm->getNumBanks());
}

TEST_CASE("load single preset into dual editbuffer")
{
  MockPresetStorage presets;

  auto editBuffer = getEditBuffer();
  {
    auto scope = createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer);

    REQUIRE(editBuffer->getType() == SoundType::Layer);
  }

  SECTION("Load Single Into I")
  {
    auto scope = createTestScope();
  }

  SECTION("Load Single Into II")
  {
  }
}