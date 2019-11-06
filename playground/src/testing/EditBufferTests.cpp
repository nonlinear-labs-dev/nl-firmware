#include <third-party/include/catch.hpp>

#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <parameters/PedalParameter.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <nltools/Types.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

inline auto getEditBuffer() -> EditBuffer*
{
  return Application::get().getPresetManager()->getEditBuffer();
}

inline auto createTestScope() -> std::unique_ptr<UNDO::TransactionCreationScope>
{
  return std::move(Application::get().getPresetManager()->getUndoScope().startTestTransaction());
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
  if(auto bank = dynamic_cast<Bank*>(presetToDelete->getParent()))
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

TEST_CASE("EditBuffer Initialized")
{
  auto eb = getEditBuffer();
  REQUIRE(eb != nullptr);
}

TEST_CASE("Simple EditBuffer Conversion")
{
  auto editBuffer = getEditBuffer();
  MockPresetStorage presets;

  auto loadSinglePreset = [&]() {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
    REQUIRE(editBuffer->getType() == SoundType::Single);
  };

  SECTION("Convert Single to Layer Sound")
  {
    loadSinglePreset();

    auto scope = createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Layer);
  }

  SECTION("Convert Single to Split Sound")
  {
    loadSinglePreset();

    auto scope = createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Split);
  }

  SECTION("Undo Convert Single to Split")
  {
    loadSinglePreset();

    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
    }

    REQUIRE(editBuffer->getType() == SoundType::Split);
    editBuffer->getParent()->getUndoScope().undo();
    REQUIRE(editBuffer->getType() == SoundType::Single);
  }

  SECTION("Undo Convert Single to Layer")
  {
    loadSinglePreset();

    {
      auto scope = createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);
    }

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    editBuffer->getParent()->getUndoScope().undo();
    REQUIRE(editBuffer->getType() == SoundType::Single);
  }
}

#warning "Hier nochmal beigehen und mängel abstellen! (convert changed + master copy addition rules)"
TEST_CASE("master groups initialization tests")
{
  auto editBuffer = getEditBuffer();
  MockPresetStorage presets;

  auto loadSinglePreset = [&]() {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
    REQUIRE(editBuffer->getType() == SoundType::Single);
  };

  loadSinglePreset();

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
  MockPresetStorage presets;

  {
    //TODO mit stephan über conversion rules reden
    auto scope = createTestScope();
    auto vgMaster = presets.getSplitPreset()->findParameterByID(10002, VoiceGroup::I);
    vgMaster->setValue(scope->getTransaction(), 0.2);
    auto globalMaster = presets.getSplitPreset()->findParameterByID(247, VoiceGroup::Global);
    globalMaster->setValue(scope->getTransaction(), 0.2);
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSplitPreset());
    REQUIRE(editBuffer->getType() == SoundType::Split);
  }

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

    {
      auto scope = createTestScope();
      REQUIRE(editBuffer->anyParameterChanged());
      presets.getSplitPreset()->copyFrom(scope->getTransaction(), editBuffer);
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }

    //Convert sound to single
    {
      auto scope = createTestScope();
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
      editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    }

    auto globalMasterVolume = editBuffer->findGlobalParameterByID(247);
    auto globalMasterTune = editBuffer->findGlobalParameterByID(248);

    REQUIRE(globalMasterVolume->getControlPositionValue() == 0.7);
    REQUIRE(globalMasterTune->getControlPositionValue() == 0.7);
  }
}

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
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }

    SECTION("Load layer preset")
    {
      auto scope = createTestScope();
      auto layerPreset = presets.getLayerPreset();
      editBuffer->undoableLoad(scope->getTransaction(), layerPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == layerPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Layer);
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }

    SECTION("Load split preset")
    {
      auto scope = createTestScope();
      auto splitPreset = presets.getSplitPreset();
      editBuffer->undoableLoad(scope->getTransaction(), splitPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == splitPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Split);
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }
  }

  REQUIRE(numBanks == pm->getNumBanks());
}

TEST_CASE("Editbuffer Contents loaded")
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
    editBuffer->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::I);

    REQUIRE(editBuffer->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
  }

  SECTION("Load Single Into II")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::II);

    REQUIRE(editBuffer->getVoiceGroupName(VoiceGroup::II) == presets.getSinglePreset()->getName());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
  }
}

void changeParameterValue(UNDO::Transaction* transaction, Parameter* param)
{
  auto currentValue = param->getControlPositionValue();

  auto incNext = param->getValue().getNextStepValue(1, {});
  auto decNext = param->getValue().getNextStepValue(-1, {});

  if(incNext != currentValue)
    param->setCPFromHwui(transaction, incNext);
  else if(decNext != currentValue)
    param->setCPFromHwui(transaction, decNext);
}

TEST_CASE("Load <-> Changed")
{
  MockPresetStorage presets;
  auto editBuffer = getEditBuffer();

  SECTION("Load leads to reset changed indication")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());

    REQUIRE(!editBuffer->anyParameterChanged());

    auto param = editBuffer->findParameterByID(2, VoiceGroup::I);

    REQUIRE(param != nullptr);
    REQUIRE(!param->isChangedFromLoaded());

    changeParameterValue(scope->getTransaction(), param);

    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());
  }

  SECTION("Load leads to reset changed indication dual")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

    REQUIRE(!editBuffer->anyParameterChanged());

    auto param = editBuffer->findParameterByID(2, VoiceGroup::I);

    REQUIRE(param != nullptr);
    REQUIRE(!param->isChangedFromLoaded());

    changeParameterValue(scope->getTransaction(), param);

    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());
  }

  SECTION("Recall")
  {
    Parameter* param{ nullptr };
    {
      auto scope = createTestScope();
      editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
      param = editBuffer->findParameterByID(2, VoiceGroup::I);
      REQUIRE(param != nullptr);
      REQUIRE(!param->isChangedFromLoaded());

      changeParameterValue(scope->getTransaction(), param);

      REQUIRE(param->isChangedFromLoaded());
    }

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());

    param->undoableRecallFromPreset();

    REQUIRE(!param->isChangedFromLoaded());
  }
}

TEST_CASE("load presets of different types")
{
  auto editBuffer = getEditBuffer();
  MockPresetStorage presets;
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Single ")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());

    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Split")
  {
    auto scope = createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSplitPreset());

    REQUIRE(editBuffer->getType() == SoundType::Split);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }
}

TEST_CASE("Voice Group Label")
{
  auto eb = getEditBuffer();

  MockPresetStorage presets;

  {
    auto scope = createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
    REQUIRE(eb->getType() == SoundType::Layer);
    eb->setVoiceGroupName(scope->getTransaction(), "I", VoiceGroup::I);
    eb->setVoiceGroupName(scope->getTransaction(), "II", VoiceGroup::II);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "I");
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "II");
  }

  SECTION("Convert to Single clears Voice Group Labels")
  {
    auto scope = createTestScope();
    eb->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I).empty());
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II).empty());
  }

  SECTION("Load Single into Voice Group sets Voice Group Label to Preset Name")
  {
    auto scope = createTestScope();
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "I");
    eb->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
  }

  SECTION("Convert Single to Dual sets Voice Group Labels to prior EB Name")
  {
    auto scope = createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
    auto ebName = eb->getName();
    eb->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == ebName);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == ebName);
  }
}