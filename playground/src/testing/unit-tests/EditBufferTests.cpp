#include <third-party/include/catch.hpp>
#include <testing/TestHelper.h>

#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <parameters/PedalParameter.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <nltools/Types.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <device-settings/RandomizeAmount.h>
#include "MockPresetStorage.h"

inline EditBuffer* getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

template <SoundType tNewSoundType> void loadPreset(Preset* newPreset)
{
  auto editBuffer = getEditBuffer();
  auto scope = TestHelper::createTestScope();
  editBuffer->undoableLoad(scope->getTransaction(), newPreset);

  REQUIRE(editBuffer->getType() == tNewSoundType);
  REQUIRE_FALSE(editBuffer->anyParameterChanged());
  REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == newPreset->getUuid());
}

TEST_CASE("EditBuffer Initialized")
{
  auto eb = getEditBuffer();
  REQUIRE(eb != nullptr);
}

TEST_CASE("Simple EditBuffer Conversion")
{
  auto editBuffer = getEditBuffer();
  MockPresetStorage presets;

  loadPreset<SoundType::Single>(presets.getSinglePreset());

  SECTION("Convert Single to Layer Sound")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Convert Single to Split Sound")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Split);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Undo Convert Single to Split")
  {
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
    }

    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE(editBuffer->getType() == SoundType::Split);
    editBuffer->getParent()->getUndoScope().undo();
    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Undo Convert Single to Layer")
  {
    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);
    }

    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
    editBuffer->getParent()->getUndoScope().undo();
    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }
}

template <VoiceGroup tVoiceGroup, typename LoadPresetFunction>
void masterVolumeDualToSingleConversionTests(LoadPresetFunction loadPresetCB, Preset* singlePreset,
                                             Preset* presetToLoad)
{
  auto editBuffer = getEditBuffer();
  auto vgMaster = editBuffer->findParameterByID(10002, tVoiceGroup);
  auto globalMaster = editBuffer->findParameterByID(247, VoiceGroup::Global);

  REQUIRE(vgMaster != nullptr);
  REQUIRE(globalMaster != nullptr);

  loadPreset<SoundType::Single>(singlePreset);

  //Prepare Split-Preset initialize Master Values
  {
    auto scope = TestHelper::createTestScope();
    auto presetMaster = presetToLoad->findParameterByID(10002, tVoiceGroup);
    REQUIRE(presetMaster != nullptr);

    presetMaster->setValue(scope->getTransaction(), 0.125);
    REQUIRE(presetMaster->getValue() == 0.125);
  }

  //Load Preset and check for our value
  {
    loadPresetCB(presetToLoad);
    REQUIRE(vgMaster->getControlPositionValue() == 0.125);
  }

  const auto globalMasterPreConversion = globalMaster->getControlPositionValue();
  const auto voiceGroupMasterIPreConversion = vgMaster->getControlPositionValue();

  //Convert and assert master conversion rules
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToSingle(scope->getTransaction(), tVoiceGroup);
    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE(vgMaster->getControlPositionValue() == vgMaster->getDefaultValue());
    REQUIRE(globalMaster->getControlPositionValue() == globalMasterPreConversion + voiceGroupMasterIPreConversion);
  }
}

template <VoiceGroup tVoiceGroup, int polyID, int globalID, typename LoadPresetFunction>
void dualToSingleTestsPolyToGlobalParameterCopy(LoadPresetFunction loadPresetCB, Preset* singlePreset,
                                                Preset* dualPreset)
{
  auto editBuffer = getEditBuffer();
  auto vgParameter = editBuffer->findParameterByID(polyID, tVoiceGroup);
  auto globalParameter = editBuffer->findParameterByID(globalID, VoiceGroup::Global);

  REQUIRE(vgParameter != nullptr);
  REQUIRE(globalParameter != nullptr);

  loadPreset<SoundType::Single>(singlePreset);

  //Prepare Split-Preset initialize Tune Values
  {
    auto scope = TestHelper::createTestScope();
    auto presetParameter = dualPreset->findParameterByID(polyID, tVoiceGroup);
    REQUIRE(presetParameter != nullptr);

    presetParameter->setValue(scope->getTransaction(), 0.125);
    REQUIRE(presetParameter->getValue() == 0.125);
  }

  //Load Preset and check for our value
  {
    loadPresetCB(dualPreset);
    REQUIRE(vgParameter->getControlPositionValue() == 0.125);
  }

  const auto globalParameterPreConversion = globalParameter->getControlPositionValue();
  const auto vGroupParameterPreConversion = vgParameter->getControlPositionValue();

  //Convert and assert tune conversion rules
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToSingle(scope->getTransaction(), tVoiceGroup);
    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE(vgParameter->getControlPositionValue() == vgParameter->getDefaultValue());
    REQUIRE(globalParameter->getControlPositionValue() == globalParameterPreConversion + vGroupParameterPreConversion);
  }
}

TEST_CASE("Sound Conversion - Master Volume Parameter")
{
  MockPresetStorage presets;

  SECTION("I Split to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 10002, 247>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("II Split to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 10002, 247>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("I Layer to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 10002, 247>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
  }

  SECTION("II Layer to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 10002, 247>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
  }
}

TEST_CASE("Sound Conversion - Master Tune Parameter")
{
  MockPresetStorage presets;

  SECTION("I Split to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 10003, 248>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("II Split to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 10003, 248>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("I Layer to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 10003, 248>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
  }

  SECTION("II Layer to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 10003, 248>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
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

  SECTION("Convert to Layer initializes Parts from VoiceGroup::I ")
  {
    {
      auto scope = TestHelper::createTestScope();
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

  SECTION("Convert to Split initializes Parts from VoiceGroup::I")
  {
    {
      auto scope = TestHelper::createTestScope();
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
      auto scope = TestHelper::createTestScope();
      auto singlePreset = presets.getSinglePreset();
      editBuffer->undoableLoad(scope->getTransaction(), singlePreset);

      REQUIRE(editBuffer->getType() == SoundType::Single);
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }

    SECTION("Load layer preset")
    {
      auto scope = TestHelper::createTestScope();
      auto layerPreset = presets.getLayerPreset();
      editBuffer->undoableLoad(scope->getTransaction(), layerPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == layerPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Layer);
      REQUIRE_FALSE(editBuffer->anyParameterChanged());
    }

    SECTION("Load split preset")
    {
      auto scope = TestHelper::createTestScope();
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
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer);

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Single Into I")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::I);

    REQUIRE(editBuffer->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Single Into II")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::II);

    REQUIRE(editBuffer->getVoiceGroupName(VoiceGroup::II) == presets.getSinglePreset()->getName());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }
}

TEST_CASE("Load <-> Changed")
{
  MockPresetStorage presets;
  auto editBuffer = getEditBuffer();

  SECTION("Load resets Changed-Indication")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());

    REQUIRE_FALSE(editBuffer->anyParameterChanged());

    auto param = editBuffer->findParameterByID(2, VoiceGroup::I);

    REQUIRE(param != nullptr);
    REQUIRE_FALSE(param->isChangedFromLoaded());

    TestHelper::forceParameterChange(scope->getTransaction(), param);

    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Dual Preset resets Changed-Indication")
  {
    auto scope = TestHelper::createTestScope();

    editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
    REQUIRE_FALSE(editBuffer->anyParameterChanged());

    auto param = editBuffer->findParameterByID(2, VoiceGroup::I);
    REQUIRE(param != nullptr);
    REQUIRE_FALSE(param->isChangedFromLoaded());

    TestHelper::forceParameterChange(scope->getTransaction(), param);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());
  }

  SECTION("Recall Poly Parameter (I) in Dual EditBuffer")
  {
    Parameter* param{ nullptr };

    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

      param = editBuffer->findParameterByID(2, VoiceGroup::I);
      REQUIRE(param != nullptr);
      REQUIRE_FALSE(param->isChangedFromLoaded());

      TestHelper::forceParameterChange(scope->getTransaction(), param);

      REQUIRE(param->isChangedFromLoaded());
    }

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());

    param->undoableRecallFromPreset();

    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE_FALSE(param->isChangedFromLoaded());
  }

  SECTION("Recall Poly Parameter (II) in Dual EditBuffer")
  {
    Parameter* param{ nullptr };

    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

      param = editBuffer->findParameterByID(15, VoiceGroup::II);
      REQUIRE(param != nullptr);
      REQUIRE_FALSE(param->isChangedFromLoaded());

      TestHelper::forceParameterChange(scope->getTransaction(), param);

      REQUIRE(param->isChangedFromLoaded());
    }

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());

    param->undoableRecallFromPreset();

    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE_FALSE(param->isChangedFromLoaded());
  }

  SECTION("Recall Global Parameter in Dual EditBuffer")
  {
    Parameter* param{ nullptr };

    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

      param = editBuffer->findParameterByID(247, VoiceGroup::Global);
      REQUIRE(param != nullptr);
      REQUIRE_FALSE(param->isChangedFromLoaded());

      TestHelper::forceParameterChange(scope->getTransaction(), param);

      REQUIRE(param->isChangedFromLoaded());
    }

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->anyParameterChanged());

    param->undoableRecallFromPreset();

    REQUIRE_FALSE(editBuffer->anyParameterChanged());
    REQUIRE_FALSE(param->isChangedFromLoaded());
  }
}

TEST_CASE("Load Presets of all types")
{
  auto editBuffer = getEditBuffer();
  MockPresetStorage presets;

  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Single")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());

    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Split")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSplitPreset());

    REQUIRE(editBuffer->getType() == SoundType::Split);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Load Layer")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }
}

TEST_CASE("Part Label")
{
  auto eb = getEditBuffer();

  MockPresetStorage presets;

  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
    REQUIRE(eb->getType() == SoundType::Layer);
    eb->setVoiceGroupName(scope->getTransaction(), "I", VoiceGroup::I);
    eb->setVoiceGroupName(scope->getTransaction(), "II", VoiceGroup::II);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "I");
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "II");
  }

  SECTION("Convert to Single clears Part Labels")
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I).empty());
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II).empty());
    REQUIRE_FALSE(eb->anyParameterChanged());
  }

  SECTION("Load Single into Part sets Part Label to Preset Name")
  {
    auto scope = TestHelper::createTestScope();
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "I");
    eb->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
  }

  SECTION("Convert Single to Dual sets Part Labels to prior EB Name")
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
    auto ebName = eb->getName();
    eb->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == ebName);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == ebName);
    REQUIRE_FALSE(eb->anyParameterChanged());
  }
}

void randomizeRequireChangedAndInitSoundTest(const Preset* preset)
{
  auto eb = TestHelper::getEditBuffer();
  {
    auto scope = TestHelper::createTestScope();

    //setup randomize amount
    Application::get().getSettings()->getSetting<RandomizeAmount>()->set(0.5);

    eb->undoableRandomize(scope->getTransaction(), Initiator::EXPLICIT_OTHER);
    REQUIRE(eb->anyParameterChanged());
    eb->undoableInitSound(scope->getTransaction());
    REQUIRE(!eb->anyParameterChanged());
    auto masterVolume = eb->findParameterByID(247, VoiceGroup::Global);
    REQUIRE(!masterVolume->isValueDifferentFrom(masterVolume->getDefaultValue()));
    auto masterTune = eb->findParameterByID(248, VoiceGroup::Global);
    REQUIRE(!masterTune->isValueDifferentFrom(masterTune->getDefaultValue()));

    for(auto& vg : { VoiceGroup::I, VoiceGroup::II })
    {
      auto vgVolume = eb->findParameterByID(10002, vg);
      REQUIRE(!vgVolume->isValueDifferentFrom(vgVolume->getDefaultValue()));
      auto vgTune = eb->findParameterByID(10003, vg);
      REQUIRE(!vgTune->isValueDifferentFrom(vgTune->getDefaultValue()));
    }
  }
}

TEST_CASE("Init Sound resets all Parameters")
{
  MockPresetStorage presets;

  SECTION("Init Single Sound")
  {
    randomizeRequireChangedAndInitSoundTest(presets.getSinglePreset());
  }

  SECTION("Init Split Preset")
  {
    randomizeRequireChangedAndInitSoundTest(presets.getSplitPreset());
  }

  SECTION("Init Layer Preset")
  {
    randomizeRequireChangedAndInitSoundTest(presets.getLayerPreset());
  }
}