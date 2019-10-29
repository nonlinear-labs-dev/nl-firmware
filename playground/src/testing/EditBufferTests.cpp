#include <third-party/include/catch.hpp>

#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <parameters/PedalParameter.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <nltools/Types.h>

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

TEST_CASE("poly groups layer sound initialized")
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