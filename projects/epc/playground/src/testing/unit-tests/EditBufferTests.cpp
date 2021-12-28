#include <catch.hpp>
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
#include "mock/MockPresetStorage.h"
#include <Application.h>
#include <device-settings/Settings.h>
#include <parameter_declarations.h>

inline EditBuffer *getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

template <SoundType tNewSoundType> void loadPreset(Preset *newPreset)
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);
  useCase.load(newPreset);
  editBuffer->TEST_doDeferredJobs();

  CHECK(editBuffer->getType() == tNewSoundType);
  CHECK_FALSE(editBuffer->isModified());
  CHECK_FALSE(editBuffer->findAnyParameterChanged());
  CHECK(editBuffer->getUUIDOfLastLoadedPreset() == newPreset->getUuid());
}

TEST_CASE("EditBuffer Initialized")
{
  auto eb = getEditBuffer();
  CHECK(eb != nullptr);
}

TEST_CASE("Simple EditBuffer Conversion")
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);
  MockPresetStorage presets;

  loadPreset<SoundType::Single>(presets.getSinglePreset());

  SECTION("Convert Single to Layer Sound")
  {
    useCase.convertToLayer(VoiceGroup::I);
    editBuffer->TEST_doDeferredJobs();

    CHECK(editBuffer->getType() == SoundType::Layer);
    CHECK(editBuffer->isModified());
    CHECK_FALSE(editBuffer->findAnyParameterChanged());
  }

  SECTION("Convert Single to Split Sound")
  {
    useCase.convertToSplit(VoiceGroup::I);
    editBuffer->TEST_doDeferredJobs();

    CHECK(editBuffer->getType() == SoundType::Split);
    CHECK(editBuffer->isModified());
    CHECK_FALSE(editBuffer->findAnyParameterChanged());

    CHECK(editBuffer->findParameterByID({ 249, VoiceGroup::I })->getValue().getFineDenominator() == 11);
    CHECK(editBuffer->findParameterByID({ 249, VoiceGroup::II })->getValue().getFineDenominator() == 11);
  }

  SECTION("Undo Convert Single to Split")
  {
    useCase.convertToSplit(VoiceGroup::I);
    editBuffer->TEST_doDeferredJobs();

    CHECK(editBuffer->isModified());
    CHECK_FALSE(editBuffer->findAnyParameterChanged());
    CHECK(editBuffer->getType() == SoundType::Split);
    CHECK(editBuffer->findParameterByID({ 249, VoiceGroup::I })->getValue().getFineDenominator() == 11);
    CHECK(editBuffer->findParameterByID({ 249, VoiceGroup::II })->getValue().getFineDenominator() == 11);

    editBuffer->getParent()->getUndoScope().undo();
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE(editBuffer->findParameterByID({ 249, VoiceGroup::I })->getValue().getFineDenominator() == 23);
    REQUIRE(editBuffer->findParameterByID({ 249, VoiceGroup::II })->getValue().getFineDenominator() == 23);  // Ignored
  }

  SECTION("Undo Convert Single to Layer")
  {
    useCase.convertToLayer(VoiceGroup::I);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE(editBuffer->findParameterByID({ 249, VoiceGroup::I })->getValue().getFineDenominator() == 11);
    REQUIRE(editBuffer->findParameterByID({ 249, VoiceGroup::II })->getValue().getFineDenominator() == 11);

    editBuffer->getParent()->getUndoScope().undo();
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE(editBuffer->findParameterByID({ 249, VoiceGroup::I })->getValue().getFineDenominator() == 23);
    REQUIRE(editBuffer->findParameterByID({ 249, VoiceGroup::II })->getValue().getFineDenominator() == 23);
  }
}

template <VoiceGroup tVoiceGroup, int polyID, int globalID, typename LoadPresetFunction>
void dualToSingleTestsPolyToGlobalParameterCopy(LoadPresetFunction loadPresetCB, Preset *singlePreset,
                                                Preset *dualPreset)
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);
  auto vgParameter = editBuffer->findParameterByID({ polyID, tVoiceGroup });
  auto globalParameter = editBuffer->findParameterByID({ globalID, VoiceGroup::Global });

  REQUIRE(vgParameter != nullptr);
  REQUIRE(globalParameter != nullptr);

  loadPreset<SoundType::Single>(singlePreset);

  //Prepare Split-Preset initialize Values
  {
    auto scope = TestHelper::createTestScope();
    auto presetParameter = dualPreset->findParameterByID({ polyID, tVoiceGroup }, true);
    REQUIRE(presetParameter != nullptr);

    presetParameter->setValue(scope->getTransaction(), 0.125);
    REQUIRE(presetParameter->getValue() == 0.125);
  }

  //Load Preset and check for our value
  {
    loadPresetCB(dualPreset);
    REQUIRE(vgParameter->getControlPositionValue() == 0.125);
  }

  const auto globalParameterPreConversion = globalParameter->getDisplayValue();
  const auto vGroupParameterPreConversion = vgParameter->getDisplayValue();

  //Convert and assert conversion rules
  {
    useCase.convertToSingle(tVoiceGroup);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE(vgParameter->getControlPositionValue() == vgParameter->getFactoryDefaultValue());
    REQUIRE(globalParameter->getDisplayValue() == globalParameterPreConversion + vGroupParameterPreConversion);
  }
}

TEST_CASE("Sound Conversion - Master Volume Parameter")
{
  MockPresetStorage presets;

  SECTION("I Split to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 358, 247>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("II Split to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 358, 247>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("I Layer to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 358, 247>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
  }

  SECTION("II Layer to Single copy Part Master Volume to Global Master Volume")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 358, 247>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
  }
}

TEST_CASE("Sound Conversion - Master Tune Parameter")
{
  MockPresetStorage presets;

  SECTION("I Split to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 360, 248>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("II Split to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 360, 248>(
        &loadPreset<SoundType::Split>, presets.getSinglePreset(), presets.getSplitPreset());
  }

  SECTION("I Layer to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::I, 360, 248>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
  }

  SECTION("II Layer to Single copy Part Tune to Global Master Tune")
  {
    dualToSingleTestsPolyToGlobalParameterCopy<VoiceGroup::II, 360, 248>(
        &loadPreset<SoundType::Layer>, presets.getSinglePreset(), presets.getLayerPreset());
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
    auto singlePreset = presets.getSinglePreset();
    auto layerPreset = presets.getLayerPreset();
    auto splitPreset = presets.getSplitPreset();

    SECTION("Load single preset")
    {
      EditBufferUseCases useCase(*editBuffer);
      useCase.load(singlePreset);

      REQUIRE(editBuffer->getType() == SoundType::Single);
      REQUIRE_FALSE(editBuffer->isModified());
      REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    }

    SECTION("Load layer preset")
    {
      EditBufferUseCases useCase(*editBuffer);
      useCase.load(layerPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == layerPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Layer);
      REQUIRE_FALSE(editBuffer->isModified());
      REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    }

    SECTION("Load split preset")
    {
      EditBufferUseCases useCase(*editBuffer);
      useCase.load(splitPreset);

      REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == splitPreset->getUuid());
      REQUIRE(editBuffer->getType() == SoundType::Split);
      REQUIRE_FALSE(editBuffer->isModified());
      REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    }
  }

  REQUIRE(numBanks == pm->getNumBanks());
}

TEST_CASE("Editbuffer Contents loaded")
{
  MockPresetStorage presets;

  auto editBuffer = getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);

  useCase.convertToLayer(VoiceGroup::I);
  REQUIRE(editBuffer->getType() == SoundType::Layer);
  REQUIRE_FALSE(editBuffer->isModified());
  REQUIRE_FALSE(editBuffer->findAnyParameterChanged());

  SECTION("Load Single Into I")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoadSinglePresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(),
                                                      VoiceGroup::I);

    REQUIRE(editBuffer->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
  }

  SECTION("Load Single Into II")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoadSinglePresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(),
                                                      VoiceGroup::II);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(editBuffer->getVoiceGroupName(VoiceGroup::II) == presets.getSinglePreset()->getName());
    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
  }
}

TEST_CASE("Load <-> Changed")
{
  MockPresetStorage presets;
  auto singlePreset = presets.getSinglePreset();
  auto layerPreset = presets.getLayerPreset();
  auto splitPreset = presets.getSplitPreset();

  auto editBuffer = getEditBuffer();

  SECTION("Load resets Changed-Indication")
  {
    EditBufferUseCases useCase(*editBuffer);
    useCase.load(singlePreset);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());

    auto param = editBuffer->findParameterByID({ 2, VoiceGroup::I });

    REQUIRE(param != nullptr);
    REQUIRE_FALSE(param->isChangedFromLoaded());

    TestHelper::forceParameterChange(param);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->isModified());
    REQUIRE(editBuffer->findAnyParameterChanged());
  }

  SECTION("Load Dual Preset resets Changed-Indication")
  {
    EditBufferUseCases useCase(*editBuffer);
    useCase.load(layerPreset);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());

    auto param = editBuffer->findParameterByID({ 2, VoiceGroup::I });
    REQUIRE(param != nullptr);
    REQUIRE_FALSE(param->isChangedFromLoaded());

    TestHelper::forceParameterChange(param);
    editBuffer->TEST_doDeferredJobs();

    REQUIRE(param->isChangedFromLoaded());

    REQUIRE(editBuffer->isModified());
    REQUIRE(editBuffer->findAnyParameterChanged());
    useCase.load(layerPreset);
    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
  }

  SECTION("Recall Poly Parameter (I) in Dual EditBuffer")
  {
    Parameter *param { nullptr };

    {
      EditBufferUseCases useCase(*editBuffer);
      useCase.load(layerPreset);

      param = editBuffer->findParameterByID({ 2, VoiceGroup::I });
      REQUIRE(param != nullptr);
      REQUIRE_FALSE(param->isChangedFromLoaded());

      TestHelper::forceParameterChange(param);
      editBuffer->TEST_doDeferredJobs();

      REQUIRE(param->isChangedFromLoaded());
    }

    ParameterUseCases useCase(param);

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->isModified());
    REQUIRE(editBuffer->findAnyParameterChanged());

    useCase.recallParameterFromPreset();
    editBuffer->TEST_doDeferredJobs();

    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE_FALSE(param->isChangedFromLoaded());
  }

  SECTION("Recall Poly Parameter (II) in Dual EditBuffer")
  {
    Parameter *param { nullptr };

    {
      EditBufferUseCases useCases(*editBuffer);
      useCases.load(layerPreset);
      editBuffer->TEST_doDeferredJobs();

      param = editBuffer->findParameterByID({ 15, VoiceGroup::II });
      REQUIRE(param != nullptr);
      REQUIRE_FALSE(param->isChangedFromLoaded());

      TestHelper::forceParameterChange(param);
      editBuffer->TEST_doDeferredJobs();

      REQUIRE(param->isChangedFromLoaded());
    }

    ParameterUseCases useCase(param);

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->isModified());
    REQUIRE(editBuffer->findAnyParameterChanged());

    useCase.recallParameterFromPreset();
    editBuffer->TEST_doDeferredJobs();

    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE_FALSE(param->isChangedFromLoaded());
  }

  SECTION("Recall Global Parameter in Dual EditBuffer")
  {
    Parameter *param { nullptr };

    {
      EditBufferUseCases useCases(*editBuffer);
      useCases.load(layerPreset);
      editBuffer->TEST_doDeferredJobs();

      param = editBuffer->findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
      REQUIRE(param != nullptr);
      REQUIRE_FALSE(param->isChangedFromLoaded());

      TestHelper::forceParameterChange(param);
      editBuffer->TEST_doDeferredJobs();

      REQUIRE(param->isChangedFromLoaded());
    }

    ParameterUseCases useCase(param);

    REQUIRE(param != nullptr);
    REQUIRE(param->isChangedFromLoaded());
    REQUIRE(editBuffer->isModified());
    REQUIRE(editBuffer->findAnyParameterChanged());

    useCase.recallParameterFromPreset();
    editBuffer->TEST_doDeferredJobs();

    REQUIRE_FALSE(editBuffer->isModified());
    REQUIRE_FALSE(editBuffer->findAnyParameterChanged());
    REQUIRE_FALSE(param->isChangedFromLoaded());
  }
}

TEST_CASE("Part Label")
{
  auto eb = getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  MockPresetStorage presets;
  auto single = presets.getSinglePreset();
  auto layer = presets.getLayerPreset();
  auto split = presets.getSplitPreset();

  {
    ebUseCases.load(layer);
    REQUIRE(eb->getType() == SoundType::Layer);

    auto scope = TestHelper::createTestScope();
    eb->setVoiceGroupName(scope->getTransaction(), "\uE071", VoiceGroup::I);
    eb->setVoiceGroupName(scope->getTransaction(), "\uE072", VoiceGroup::II);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "\uE071");
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "\uE072");
  }

  SECTION("Convert to Single clears Part Labels")
  {
    ebUseCases.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I).empty());
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II).empty());
    REQUIRE_FALSE(eb->isModified());
    REQUIRE_FALSE(eb->findAnyParameterChanged());
  }

  SECTION("Load Single into Part sets Part Label to Preset Name")
  {
    auto scope = TestHelper::createTestScope();
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "\uE071");
    eb->undoableLoadSinglePresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
  }

  SECTION("Convert Single to Dual sets Part Labels to prior EB Name")
  {
    ebUseCases.load(single);

    auto ebName = eb->getName();
    ebUseCases.convertToSplit(VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == ebName);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == ebName);
    REQUIRE_FALSE(eb->isModified());
    REQUIRE_FALSE(eb->findAnyParameterChanged());
  }
}

void randomizeRequireChangedAndInitSoundTest(const Preset *preset)
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCases(*eb);

  useCases.randomize(0.5);
  eb->TEST_doDeferredJobs();
  REQUIRE(eb->isModified());
  REQUIRE(eb->findAnyParameterChanged());

  useCases.initSound(Defaults::FactoryDefault);
  REQUIRE_FALSE(eb->isModified());
  REQUIRE_FALSE(eb->findAnyParameterChanged());

  auto masterVolume = eb->findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  REQUIRE(!masterVolume->isValueDifferentFrom(masterVolume->getFactoryDefaultValue()));
  auto masterTune = eb->findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global });
  REQUIRE(!masterTune->isValueDifferentFrom(masterTune->getFactoryDefaultValue()));

  for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
  {
    auto vgVolume = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, vg });
    REQUIRE(!vgVolume->isValueDifferentFrom(vgVolume->getFactoryDefaultValue()));
    auto vgTune = eb->findParameterByID({ C15::PID::Voice_Grp_Tune, vg });
    REQUIRE(!vgTune->isValueDifferentFrom(vgTune->getFactoryDefaultValue()));
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

TEST_CASE("Convert Sound Leads to Converted UUID")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  ebUseCases.convertToSingle(VoiceGroup::I);

  MockPresetStorage presets;
  auto single = presets.getSinglePreset();
  auto layer = presets.getLayerPreset();
  auto split = presets.getSplitPreset();

  SECTION("Convert from Layer to Single")
  {
    ebUseCases.load(layer);
    REQUIRE(eb->getType() == SoundType::Layer);
    ebUseCases.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getUUIDOfLastLoadedPreset() == Uuid::converted());
  }

  SECTION("Convert from Split to Single")
  {
    ebUseCases.load(split);
    REQUIRE(eb->getType() == SoundType::Split);
    ebUseCases.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getUUIDOfLastLoadedPreset() == Uuid::converted());
  }

  SECTION("Convert from Single to Layer")
  {
    ebUseCases.load(single);
    REQUIRE(eb->getType() == SoundType::Single);
    ebUseCases.convertToLayer(VoiceGroup::I);
    REQUIRE(eb->getUUIDOfLastLoadedPreset() == Uuid::converted());
  }

  SECTION("Convert from Single to Split")
  {
    ebUseCases.load(single);
    REQUIRE(eb->getType() == SoundType::Single);
    ebUseCases.convertToSplit(VoiceGroup::I);
    REQUIRE(eb->getUUIDOfLastLoadedPreset() == Uuid::converted());
  }
}

namespace PartNaming
{
  void renamePresetPartsToEinsAndZwo(Preset *p)
  {
    auto scope = TestHelper::createTestScope();
    p->undoableSetVoiceGroupName(scope->getTransaction(), VoiceGroup::I, "Eins");
    p->undoableSetVoiceGroupName(scope->getTransaction(), VoiceGroup::II, "Zwo");
    REQUIRE(p->getVoiceGroupName(VoiceGroup::I) == "Eins");
    REQUIRE(p->getVoiceGroupName(VoiceGroup::II) == "Zwo");
  }

  template <typename tStr> void renamePresetTo(Preset *p, const tStr &s)
  {
    auto scope = TestHelper::createTestScope();
    p->setName(scope->getTransaction(), s);
    REQUIRE(p->getName() == s);
  }
}

TEST_CASE("Convert Dual To Single Label Naming")
{
  MockPresetStorage presets;
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  auto layerPreset = presets.getLayerPreset();
  auto splitPreset = presets.getSplitPreset();
  auto singlePreset = presets.getSinglePreset();

  PartNaming::renamePresetPartsToEinsAndZwo(layerPreset);
  PartNaming::renamePresetPartsToEinsAndZwo(splitPreset);

  SECTION("Convert Layer I to Single")
  {
    ebUseCases.load(layerPreset);
    ebUseCases.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getName() == "Eins conv.");
  }

  SECTION("Convert Layer II to Single")
  {
    ebUseCases.load(layerPreset);
    ebUseCases.convertToSingle(VoiceGroup::II);
    REQUIRE(eb->getName() == "Zwo conv.");
  }

  SECTION("Convert Split I to Single")
  {
    ebUseCases.load(splitPreset);
    ebUseCases.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getName() == "Eins conv.");
  }

  SECTION("Convert Split II to Single")
  {
    ebUseCases.load(splitPreset);
    ebUseCases.convertToSingle(VoiceGroup::II);
    REQUIRE(eb->getName() == "Zwo conv.");
  }

  SECTION("Convert Single to Layer")
  {
    PartNaming::renamePresetTo(singlePreset, "foo");
    ebUseCases.load(singlePreset);
    ebUseCases.convertToLayer(VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "foo");
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "foo");
  }

  SECTION("Convert Single to Split")
  {
    PartNaming::renamePresetTo(singlePreset, "foo");
    ebUseCases.load(singlePreset);
    ebUseCases.convertToSplit(VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "foo");
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "foo");
  }
}

TEST_CASE("Init Sound initializes VG Labels to 'init'")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);

  MockPresetStorage presets;
  auto split = presets.getSplitPreset();
  auto layer = presets.getLayerPreset();
  auto single = presets.getSinglePreset();

  WHEN("Split Sound Init")
  {
    useCase.load(split);
    useCase.initSound(Defaults::FactoryDefault);

    THEN("Labels initialized")
    {
      REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "Init");
      REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "Init");
    }
  }

  WHEN("Layer Sound Init")
  {
    useCase.load(layer);
    useCase.initSound(Defaults::FactoryDefault);

    THEN("Labels initialized")
    {
      REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "Init");
      REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "Init");
    }
  }

  WHEN("Single Sound Init")
  {
    useCase.load(single);
    useCase.initSound(Defaults::FactoryDefault);

    THEN("Labels initialized")
    {
      REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "Init");
      REQUIRE(eb->getVoiceGroupName(VoiceGroup::II) == "Init");
    }
  }
}

TEST_CASE("Rename MC leads to changed EB")
{
  TestHelper::initSingleEditBuffer();
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();
  REQUIRE_FALSE(eb->findAnyParameterChanged());

  auto mc = dynamic_cast<MacroControlParameter *>(eb->findParameterByID({ C15::PID::MC_A, VoiceGroup::Global }));
  REQUIRE(mc != nullptr);

  WHEN("MC Renamed -> EB Changed")
  {
    mc->undoableSetGivenName(transaction, "Foo Bar");
    REQUIRE(eb->findAnyParameterChanged());
  }

  WHEN("MC Info changed -> EB Changed")
  {
    mc->undoableSetInfo(transaction, "Foo Bar");
    REQUIRE(eb->findAnyParameterChanged());
  }
}
