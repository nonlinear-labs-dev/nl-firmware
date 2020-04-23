#include <iostream>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <parameters/scale-converters/ParabolicGainDbScaleConverter.h>
#include "testing/TestHelper.h"
#include "testing/unit-tests/mock/EditBufferNamedLogicalParts.h"
#include "proxies/hwui/HWUI.h"
#include "presets/Preset.h"
#include "presets/PresetParameter.h"
#include "parameters/SplitPointParameter.h"
#include "parameters/scale-converters/ScaleConverter.h"

using EBL = EditBufferLogicalParts;

TEST_CASE("Load Single into Layer Part I")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getSinglePreset();

  REQUIRE(preset->getType() == SoundType::Single);

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::I);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);
    volume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    volume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 1);  // <- setting cp in range 0..23

    TestHelper::changeAllParameters(transaction);
  }

  WHEN("Load")
  {
    const auto localSpecialIIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>(), EBL::getToFX<VoiceGroup::II>());

    const auto toFXIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::I>());

    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto oldFadeIHash
        = EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::I>());

    const auto oldFadeIIHash
        = EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>());

    const auto oldCrossFBIHash = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>());

    const auto oldUnisonIHash = EBL::createValueHash(EBL::getUnison<VoiceGroup::I>(), EBL::getMono<VoiceGroup::I>());

    eb->undoableLoadSinglePreset(preset, VoiceGroup::I);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Layer);
    }

    THEN("Local Special II unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>(), EBL::getToFX<VoiceGroup::II>())
            == localSpecialIIHash);
    }

    THEN("Local Special I unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>()) == oldCrossFBIHash);
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::I>()) == toFXIHash);
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::I }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono I Untouched")
    {
      CHECK(oldUnisonIHash == EBL::createValueHash(EBL::getUnison<VoiceGroup::I>(), EBL::getMono<VoiceGroup::I>()));
    }

    THEN("Fade I/II is untouched")
    {
      CHECK(EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::I>())
            == oldFadeIHash);
      CHECK(EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>())
            == oldFadeIIHash);
    }

    THEN("Unison and Mono II is default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getUnison<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getMono<VoiceGroup::II>()));
    }

    THEN("Part Volume and Tune correct")
    {
      auto volumeScale
          = static_cast<const ParabolicGainDbScaleConverter*>(EBL::getMasterVolume()->getValue().getScaleConverter());
      PresetParameter* vol = nullptr;
      CHECK_NOTHROW(vol = preset->findParameterByID({ 247, VoiceGroup::Global }, true));
      CHECK(vol != nullptr);

      auto tuneScale = EBL::getMasterTune()->getValue().getScaleConverter();
      PresetParameter* tune = nullptr;
      CHECK_NOTHROW(tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true));
      CHECK(tune != nullptr);

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());

      auto calculatedVolumeCP = volumeScale->displayToControlPosition(presetGlobalVolumeDisplay - oldVolumeDisplay);

      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getPartVolume<VoiceGroup::I>(), calculatedVolumeCP);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      CHECK(EBL::getPartTune<VoiceGroup::I>()->getDisplayValue() == Approx(presetGlobalTuneDisplay - oldTuneDisplay));

      CHECK(EBL::getPartVolume<VoiceGroup::I>()->getModulationSource() == MacroControls::MC1);
      CHECK(EBL::getPartVolume<VoiceGroup::I>()->getModulationAmount() == 1);
    }

    THEN("Global Groups unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getMaster()) == oldMasterHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == oldMCMHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == oldScaleHash);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Load Single into Layer Part II")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getSinglePreset();

  REQUIRE(preset->getType() == SoundType::Single);

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);
    volume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    volume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 1);  // <- setting cp in range 0..23

    TestHelper::changeAllParameters(transaction);
  }

  WHEN("Load")
  {
    const auto localSpecialIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>());
    const auto toFXIIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());

    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto oldFadeIHash
        = EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::I>());

    const auto oldFadeIIHash
        = EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>());

    const auto oldCrossFBIIHash = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>());

    const auto oldUnisonIHash = EBL::createValueHash(EBL::getUnison<VoiceGroup::I>(), EBL::getMono<VoiceGroup::I>());

    eb->undoableLoadSinglePreset(preset, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Layer);
    }

    THEN("Local Special unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>()) == localSpecialIHash);
    }

    THEN("Cross FB and to fx II ignored")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>()) == oldCrossFBIIHash);
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::II>()) == toFXIIHash);
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono I Untouched")
    {
      CHECK(oldUnisonIHash == EBL::createValueHash(EBL::getUnison<VoiceGroup::I>(), EBL::getMono<VoiceGroup::I>()));
    }

    THEN("Fade I/II is untouched")
    {
      CHECK(EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::I>())
            == oldFadeIHash);
      CHECK(EBL::createValueHash(EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>())
            == oldFadeIIHash);
    }

    THEN("Unison and Mono II is default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getUnison<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getMono<VoiceGroup::II>()));
    }

    THEN("Part Volume and Tune correct")
    {
      auto volumeScale
          = static_cast<const ParabolicGainDbScaleConverter*>(EBL::getMasterVolume()->getValue().getScaleConverter());
      PresetParameter* vol = nullptr;
      CHECK_NOTHROW(vol = preset->findParameterByID({ 247, VoiceGroup::Global }, true));
      CHECK(vol != nullptr);

      auto tuneScale = EBL::getMasterTune()->getValue().getScaleConverter();
      PresetParameter* tune = nullptr;
      CHECK_NOTHROW(tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true));
      CHECK(tune != nullptr);

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());

      auto calculatedVolumeCP = volumeScale->displayToControlPosition(presetGlobalVolumeDisplay - oldVolumeDisplay);

      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getPartVolume<VoiceGroup::II>(), calculatedVolumeCP);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue() == Approx(presetGlobalTuneDisplay - oldTuneDisplay));

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationSource() == MacroControls::MC1);
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount() == 1);
    }

    THEN("Global Groups unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getMaster()) == oldMasterHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == oldMCMHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == oldScaleHash);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}
