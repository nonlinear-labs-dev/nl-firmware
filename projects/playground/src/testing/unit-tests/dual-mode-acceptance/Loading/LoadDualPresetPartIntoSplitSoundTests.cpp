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

TEST_CASE("Load Part I of Split into Split Part I")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getSplitPreset();

  REQUIRE(preset->getType() == SoundType::Split);

  REQUIRE_NOTHROW(preset->findParameterByID({ 356, VoiceGroup::Global }, true));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::I);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);

    auto tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true);
    tune->setValue(transaction, 0.265);

    auto partVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true);
    partVolume->setValue(transaction, 0.5);
    partVolume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    partVolume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 0.25);  // <- setting cp in range 0..23

    for(auto& p : EBL::getToFX<VoiceGroup::I>())
    {
      if(auto toFxP = preset->findParameterByID(p->getID(), false))
      {
        toFxP->setValue(transaction, 0.7);
      }
      else
      {
        REQUIRE(false);
      }
    }

    TestHelper::changeAllParameters(transaction);
  }

  WHEN("Load")
  {
    const auto toFXIIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto localNormalIIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>());

    const auto oldFadeIIHash
        = EBL::getFadeFrom<VoiceGroup::II>()->getHash() + EBL::getFadeRange<VoiceGroup::II>()->getHash();

    const auto oldUnisonMonoIIHash
        = EBL::createValueHash(EBL::getMono<VoiceGroup::II>(), EBL::getUnison<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadToPart(transaction, preset, VoiceGroup::I, VoiceGroup::I);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("toFX and local normal II untouched")
    {
      CHECK(localNormalIIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>()));
      CHECK(toFXIIHash == EBL::createValueHash(EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("ToFX I copied / Cross FB I default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));

      for(auto& p : EBL::getToFX<VoiceGroup::I>())
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.7);
    }

    THEN("Cross FB II is Default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::I }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono of I Copied to I")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getUnisonVoice<VoiceGroup::I>(), 0.5);  //compare cp for 0..11
    }

    THEN("Unison and Mono of II unchanged")
    {
      CHECK(oldUnisonMonoIIHash
            == EBL::createValueHash(EBL::getMono<VoiceGroup::II>(), EBL::getUnison<VoiceGroup::II>()));
    }

    THEN("Fade II untouched")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->getHash() + EBL::getFadeRange<VoiceGroup::II>()->getHash()
            == oldFadeIIHash);
    }

    THEN("Fade I Default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::I>() }));
    }

    THEN("Part Volume and Tune correct")
    {
      auto volumeScale = EBL::getMasterVolume()->getValue().getScaleConverter();
      PresetParameter* vol = nullptr;
      CHECK_NOTHROW(vol = preset->findParameterByID({ 247, VoiceGroup::Global }, true));
      CHECK(vol != nullptr);

      auto tuneScale = EBL::getMasterTune()->getValue().getScaleConverter();
      PresetParameter* tune = nullptr;
      CHECK_NOTHROW(tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true));
      CHECK(tune != nullptr);

      PresetParameter* ogPartTune = nullptr;
      CHECK_NOTHROW(ogPartTune = preset->findParameterByID({ 360, VoiceGroup::I }, true));
      CHECK(ogPartTune != nullptr);

      PresetParameter* ogPartVolume = nullptr;
      CHECK_NOTHROW(ogPartVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true));
      CHECK(ogPartVolume != nullptr);

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());
      auto presetPartVolumeDisplay = volumeScale->controlPositionToDisplay(ogPartVolume->getValue());

      CHECK(EBL::getPartVolume<VoiceGroup::I>()->getDisplayValue()
            == Approx((presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay).epsilon(0.005));

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(
          EBL::getPartTune<VoiceGroup::I>()->getDisplayValue()
          == Approx(std::min((presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay, 48.0)).epsilon(0.005));

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

TEST_CASE("Load Part I of Split into Split Part II")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getSplitPreset();

  REQUIRE(preset->getType() == SoundType::Split);

  REQUIRE_NOTHROW(preset->findParameterByID({ 356, VoiceGroup::Global }, true));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);

    auto tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true);
    tune->setValue(transaction, 0.265);

    auto partVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true);
    partVolume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    partVolume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 0.25);  // <- setting cp in range 0..23

    for(auto& p : EBL::getToFX<VoiceGroup::I>())
    {
      if(auto toFxP = preset->findParameterByID(p->getID(), false))
      {
        toFxP->setValue(transaction, 0.7);
      }
      else
      {
        REQUIRE(false);
      }
    }

    TestHelper::changeAllParameters(transaction);
  }

  WHEN("Load")
  {
    const auto toFXIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::I>());
    const auto toFXIIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto localNormalIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>());

    const auto oldFadeIHash
        = EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash();

    const auto oldUnisonMonoIHash
        = EBL::createValueHash(EBL::getMono<VoiceGroup::I>(), EBL::getUnison<VoiceGroup::I>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadToPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Local I untouched")
    {
      CHECK(localNormalIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
    }

    THEN("Cross FB I is default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
    }

    THEN("toFX I unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::I>()) == toFXIHash);
    }

    THEN("ToFX II copied / Cross FB II default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));

      for(auto& p : EBL::getToFX<VoiceGroup::II>())
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.7);
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono of I Copied to II")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getUnisonVoice<VoiceGroup::II>(), 0.5);  //compare cp for 0..11
    }

    THEN("Unison and Mono of I unchanged")
    {
      CHECK(oldUnisonMonoIHash == EBL::createValueHash(EBL::getMono<VoiceGroup::I>(), EBL::getUnison<VoiceGroup::I>()));
    }

    THEN("Fade I untouched")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash()
            == oldFadeIHash);
    }

    THEN("Fade II Default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>() }));
    }

    THEN("Part Volume and Tune correct")
    {
      auto volumeScale = EBL::getMasterVolume()->getValue().getScaleConverter();
      PresetParameter* vol = nullptr;
      CHECK_NOTHROW(vol = preset->findParameterByID({ 247, VoiceGroup::Global }, true));
      CHECK(vol != nullptr);

      auto tuneScale = EBL::getMasterTune()->getValue().getScaleConverter();
      PresetParameter* tune = nullptr;
      CHECK_NOTHROW(tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true));
      CHECK(tune != nullptr);

      PresetParameter* ogPartTune = nullptr;
      CHECK_NOTHROW(ogPartTune = preset->findParameterByID({ 360, VoiceGroup::I }, true));
      CHECK(ogPartTune != nullptr);

      PresetParameter* ogPartVolume = nullptr;
      CHECK_NOTHROW(ogPartVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true));
      CHECK(ogPartVolume != nullptr);

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());
      auto presetPartVolumeDisplay = volumeScale->controlPositionToDisplay(ogPartVolume->getValue());

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue()
            == Approx((presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay).epsilon(0.005));

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue()
            == Approx((presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay).epsilon(0.005));

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

TEST_CASE("Load Part I of Layer into Split Part I")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getLayerPreset();

  REQUIRE(preset->getType() == SoundType::Layer);

  REQUIRE_NOTHROW(preset->findParameterByID({ 356, VoiceGroup::Global }, true));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);

    auto tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true);
    tune->setValue(transaction, 0.265);

    auto partVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true);
    partVolume->setValue(transaction, 0.187);
    partVolume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    partVolume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 0.5);  // <- setting cp in range 0..23

    auto monoEnable = preset->findParameterByID({ 364, VoiceGroup::I }, true);
    monoEnable->setValue(transaction, 1);

    for(auto p : EBL::getToFX<VoiceGroup::I>())
    {
      auto toFX = preset->findParameterByID({ p->getID().getNumber(), VoiceGroup::I }, true);
      toFX->setValue(transaction, 0.187);
    }

    TestHelper::changeAllParameters(transaction);
  }

  WHEN("Load")
  {
    const auto oldToFXIIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto localNormalIIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>());

    const auto oldUnisonMonoIIHash
        = EBL::createValueHash(EBL::getMono<VoiceGroup::II>(), EBL::getUnison<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadToPart(transaction, preset, VoiceGroup::I, VoiceGroup::I);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Cross FB is default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("toFX and local II unchanged")
    {
      CHECK(localNormalIIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>()));
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::II>()) == oldToFXIIHash);
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::I }), 0.666);
    }

    THEN("ToFX I copied from Preset")
    {
      for(auto& p : EBL::getToFX<VoiceGroup::I>())
      {
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.187);
      }
    }

    THEN("Fade default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>() }));
      CHECK(EBL::isDefaultLoaded({ EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::I>() }));
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono I are copied From Unison/Mono I of Preset")
    {
      CHECK(EBL::getUnisonVoice<VoiceGroup::I>()->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
    }

    THEN("Unison and Mono of II unchanged")
    {
      CHECK(oldUnisonMonoIIHash
            == EBL::createValueHash(EBL::getMono<VoiceGroup::II>(), EBL::getUnison<VoiceGroup::II>()));
    }

    THEN("Part Volume and Tune correct")
    {
      auto volumeScale = EBL::getMasterVolume()->getValue().getScaleConverter();
      PresetParameter* vol = nullptr;
      CHECK_NOTHROW(vol = preset->findParameterByID({ 247, VoiceGroup::Global }, true));
      CHECK(vol != nullptr);

      auto tuneScale = EBL::getMasterTune()->getValue().getScaleConverter();
      PresetParameter* tune = nullptr;
      CHECK_NOTHROW(tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true));
      CHECK(tune != nullptr);

      PresetParameter* ogPartTune = nullptr;
      CHECK_NOTHROW(ogPartTune = preset->findParameterByID({ 360, VoiceGroup::I }, true));
      CHECK(ogPartTune != nullptr);

      PresetParameter* ogPartVolume = nullptr;
      CHECK_NOTHROW(ogPartVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true));
      CHECK(ogPartVolume != nullptr);

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());
      auto presetPartVolumeDisplay = volumeScale->controlPositionToDisplay(ogPartVolume->getValue());

      CHECK(EBL::getPartVolume<VoiceGroup::I>()->getDisplayValue()
            == Approx((presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay).epsilon(0.005));

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::I>()->getDisplayValue()
            == Approx(std::min((presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay, 48.0)));

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

TEST_CASE("Load Part I of Layer into Split Part II")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getLayerPreset();

  REQUIRE(preset->getType() == SoundType::Layer);

  REQUIRE_NOTHROW(preset->findParameterByID({ 356, VoiceGroup::Global }, true));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);

    auto tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true);
    tune->setValue(transaction, 0.265);

    auto partVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true);
    partVolume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    partVolume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 0.5);  // <- setting cp in range 0..23

    auto monoEnable = preset->findParameterByID({ 364, VoiceGroup::I }, true);
    monoEnable->setValue(transaction, 1);

    for(auto p : EBL::getToFX<VoiceGroup::I>())
    {
      auto toFX = preset->findParameterByID({ p->getID().getNumber(), VoiceGroup::I }, true);
      toFX->setValue(transaction, 0.187);
    }

    TestHelper::changeAllParameters(transaction);
  }

  WHEN("Load")
  {
    const auto toFXIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::I>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto localNormalIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>());

    const auto oldUnisonMonoIHash
        = EBL::createValueHash(EBL::getMono<VoiceGroup::I>(), EBL::getUnison<VoiceGroup::I>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadToPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("toFX and local I unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::I>()) == toFXIHash);
      CHECK(localNormalIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("ToFX II copied from Preset")
    {
      for(auto& p : EBL::getToFX<VoiceGroup::II>())
      {
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.187);
      }
    }

    THEN("Cross FB default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("Fade II default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>() }));
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono II are copied From Unison/Mono I of Preset")
    {
      CHECK(EBL::getUnisonVoice<VoiceGroup::II>()->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::II>()->getDisplayString() == "On");
    }

    THEN("Unison and Mono of I unchanged")
    {
      CHECK(oldUnisonMonoIHash == EBL::createValueHash(EBL::getMono<VoiceGroup::I>(), EBL::getUnison<VoiceGroup::I>()));
    }

    THEN("Part Volume and Tune correct")
    {
      auto volumeScale = EBL::getMasterVolume()->getValue().getScaleConverter();
      PresetParameter* vol = nullptr;
      CHECK_NOTHROW(vol = preset->findParameterByID({ 247, VoiceGroup::Global }, true));
      CHECK(vol != nullptr);

      auto tuneScale = EBL::getMasterTune()->getValue().getScaleConverter();
      PresetParameter* tune = nullptr;
      CHECK_NOTHROW(tune = preset->findParameterByID({ 248, VoiceGroup::Global }, true));
      CHECK(tune != nullptr);

      PresetParameter* ogPartTune = nullptr;
      CHECK_NOTHROW(ogPartTune = preset->findParameterByID({ 360, VoiceGroup::I }, true));
      CHECK(ogPartTune != nullptr);

      PresetParameter* ogPartVolume = nullptr;
      CHECK_NOTHROW(ogPartVolume = preset->findParameterByID({ 358, VoiceGroup::I }, true));
      CHECK(ogPartVolume != nullptr);

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());
      auto presetPartVolumeDisplay = volumeScale->controlPositionToDisplay(ogPartVolume->getValue());

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue()
            == Approx((presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay).epsilon(0.005));

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue()
            == Approx((presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay).epsilon(0.005));

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