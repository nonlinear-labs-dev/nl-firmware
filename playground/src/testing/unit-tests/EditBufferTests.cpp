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
#include "MockPresetStorage.h"

EditBuffer* getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
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

  auto loadSinglePreset = [&]() {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
    REQUIRE(editBuffer->getType() == SoundType::Single);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  };

  SECTION("Convert Single to Layer Sound")
  {
    loadSinglePreset();

    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Layer);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Convert Single to Split Sound")
  {
    loadSinglePreset();

    auto scope = TestHelper::createTestScope();
    editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);

    REQUIRE(editBuffer->getType() == SoundType::Split);
    REQUIRE_FALSE(editBuffer->anyParameterChanged());
  }

  SECTION("Undo Convert Single to Split")
  {
    loadSinglePreset();

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
    loadSinglePreset();

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

void loadSinglePreset(MockPresetStorage& presets)
{
  auto editBuffer = getEditBuffer();
  auto scope = TestHelper::createTestScope();
  editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
  REQUIRE(editBuffer->getType() == SoundType::Single);
  REQUIRE_FALSE(editBuffer->anyParameterChanged());
  REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == presets.getSinglePreset()->getUuid());
}

void loadSplitPreset(MockPresetStorage& presets)
{
  auto editBuffer = getEditBuffer();
  auto scope = TestHelper::createTestScope();
  editBuffer->undoableLoad(scope->getTransaction(), presets.getSplitPreset());
  REQUIRE(editBuffer->getType() == SoundType::Split);
  REQUIRE_FALSE(editBuffer->anyParameterChanged());
  REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == presets.getSplitPreset()->getUuid());
}

void loadLayerPreset(MockPresetStorage& presets)
{
  auto editBuffer = getEditBuffer();
  auto scope = TestHelper::createTestScope();
  editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
  REQUIRE(editBuffer->getType() == SoundType::Layer);
  REQUIRE_FALSE(editBuffer->anyParameterChanged());
  REQUIRE(editBuffer->getUUIDOfLastLoadedPreset() == presets.getLayerPreset()->getUuid());
}

#warning "Hier nochmal beigehen und mängel abstellen! (convert changed + master copy addition rules)"
TEST_CASE("Sound Conversion - Master Groups")
{
  auto editBuffer = getEditBuffer();
  MockPresetStorage presets;

  SECTION("Split to Single copy Voice Group Master Volume to Global Master Volume")
  {
    {
      auto scope = TestHelper::createTestScope();
      auto split = presets.getSplitPreset();
      auto vgVolumeI = split->findParameterByID(10002, VoiceGroup::I);
      REQUIRE(vgVolumeI != nullptr);
      vgVolumeI->setValue(scope->getTransaction(), 0.125);
      REQUIRE(vgVolumeI->getValue() == 0.125);
    }

    {
      loadSplitPreset(presets);
      auto vgVolumeI = editBuffer->findParameterByID(10002, VoiceGroup::I);
      REQUIRE(vgVolumeI != nullptr);
      REQUIRE(vgVolumeI->getControlPositionValue() == 0.125);
    }

    {
      auto scope = TestHelper::createTestScope();
      editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);

      auto vgVolumeI = editBuffer->findParameterByID(10002, VoiceGroup::I);
      REQUIRE(vgVolumeI != nullptr);
      REQUIRE(vgVolumeI->getControlPositionValue() == vgVolumeI->getDefaultValue());
    }
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

  SECTION("Convert to Layer initializes Voice Groups from VoiceGroup::I ")
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

  SECTION("Convert to Split initializes Voice Groups from VoiceGroup::I")
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

    REQUIRE(!editBuffer->anyParameterChanged());

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

TEST_CASE("Voice Group Label")
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

  SECTION("Convert to Single clears Voice Group Labels")
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I).empty());
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::II).empty());
    REQUIRE_FALSE(eb->anyParameterChanged());
  }

  SECTION("Load Single into Voice Group sets Voice Group Label to Preset Name")
  {
    auto scope = TestHelper::createTestScope();
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == "I");
    eb->undoableLoadPresetIntoDualSound(scope->getTransaction(), presets.getSinglePreset(), VoiceGroup::I);
    REQUIRE(eb->getVoiceGroupName(VoiceGroup::I) == presets.getSinglePreset()->getName());
  }

  SECTION("Convert Single to Dual sets Voice Group Labels to prior EB Name")
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