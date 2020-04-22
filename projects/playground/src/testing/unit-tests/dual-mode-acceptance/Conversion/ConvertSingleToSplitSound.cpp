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

namespace detail
{
  auto randomizeCrossFBAndToFX = [](auto transaction) {
    for(auto p : EBL::getCrossFB<VoiceGroup::I>())
    {
      p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
    }

    for(auto p : EBL::getToFX<VoiceGroup::I>())
    {
      p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
    }

    for(auto p : EBL::getCrossFB<VoiceGroup::II>())
    {
      p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
    }

    for(auto p : EBL::getToFX<VoiceGroup::II>())
    {
      p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
    }
  };

  auto randomizeFadeParams = [](auto transaction) {
    TestHelper::forceParameterChange(transaction, EBL::getFadeFrom<VoiceGroup::I>());
    TestHelper::forceParameterChange(transaction, EBL::getFadeFrom<VoiceGroup::II>());
    TestHelper::forceParameterChange(transaction, EBL::getFadeRange<VoiceGroup::I>());
    TestHelper::forceParameterChange(transaction, EBL::getFadeRange<VoiceGroup::II>());
  };

  auto changeAllParameters = [](auto transaction) {
    auto eb = TestHelper::getEditBuffer();
    eb->forEachParameter([&](Parameter* param) { TestHelper::forceParameterChange(transaction, param); });
  };
}

TEST_CASE("Convert Single Sound to Split")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  auto globalVolume = EBL::getMasterVolume();

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    TestHelper::initSingleEditBuffer(transaction);
    voicesI->loadDefault(transaction);
    voicesI->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesI->getDisplayString() == "13 voices");

    monoI->setCPFromHwui(transaction, 1);
    CHECK(monoI->getDisplayString() == "On");

    globalVolume->setModulationAmount(transaction, 0.45);
    globalVolume->setModulationSource(transaction, MacroControls::MC2);
    globalVolume->setCPFromHwui(transaction, 0.187);

    detail::randomizeCrossFBAndToFX(transaction);
    detail::randomizeFadeParams(transaction);
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());
    const auto localIMono = EBL::createHashOfVector(EBL::getMono<VoiceGroup::I>());

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto globalMasterHash = EBL::createValueHash(EBL::getMaster());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToDual(transaction, SoundType::Split);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(voicesII->getDisplayString() == "12 voices");
    }

    THEN("Special Local Params are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Local normal I have been copied to II")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>()) == localIHash);
    }

    THEN("Mono I copied to II")
    {
      CHECK(EBL::createHashOfVector(EBL::getMono<VoiceGroup::II>()) == localIMono);
      CHECK(monoI->getDisplayString() == "On");
      CHECK(monoII->getDisplayString() == "On");
    }

    THEN("Split is default")
    {
      CHECK(EBL::getSplitPoint()->isDefaultLoaded());
    }

    THEN("Global Master is Default")
    {
      CHECK(EBL::getMasterVolume()->isDefaultLoaded());
      CHECK(EBL::getMasterTune()->isDefaultLoaded());
    }

    THEN("Part Master have Preset Global Master Values")
    {
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>()) == globalMasterHash);
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>()) == globalMasterHash);
    }

    THEN("Macro Mappings are same")
    {
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("Scale unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Convert Single Sound to Layer")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();
  auto globalVolume = EBL::getMasterVolume();

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    TestHelper::initSingleEditBuffer(transaction);
    voicesI->loadDefault(transaction);
    voicesI->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesI->getDisplayString() == "13 voices");

    monoI->setCPFromHwui(transaction, 1);
    CHECK(monoI->getDisplayString() == "On");

    globalVolume->setModulationAmount(transaction, 0.45);
    globalVolume->setModulationSource(transaction, MacroControls::MC2);
    globalVolume->setCPFromHwui(transaction, 0.187);

    detail::randomizeCrossFBAndToFX(transaction);
    detail::randomizeFadeParams(transaction);
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto oldMasterHash = EBL::createValueHash(EBL::getMaster());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToDual(transaction, SoundType::Layer);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
    }

    THEN("Special Local Params are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Local normal I have been copied to II")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>()) == localIHash);
    }

    THEN("Voice Parameters of II are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getMono<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getUnison<VoiceGroup::II>()));
    }

    THEN("Part Master I/II was Copied from Global Master")
    {
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>()) == oldMasterHash);
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>()) == oldMasterHash);
    }

    THEN("Split is default")
    {
      CHECK(EBL::getSplitPoint()->isDefaultLoaded());
    }

    THEN("Global Master is Default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getMaster()));
    }

    THEN("Macro Mappings are same")
    {
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("Scale unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Convert Split (II) to Single")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  auto globalVolume = EBL::getMasterVolume();
  auto globalTune = EBL::getMasterTune();

  auto envAIAttack = EBL::getParameter({ 0, VoiceGroup::I });
  auto envAIIAttack = EBL::getParameter({ 0, VoiceGroup::II });

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);

    voicesII->loadDefault(transaction);
    voicesII->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesII->getDisplayString() == "12 voices");

    monoII->setCPFromHwui(transaction, 1);
    CHECK(monoII->getDisplayString() == "On");

    globalVolume->setCPFromHwui(transaction, 0.420);
    globalVolume->setModulationAmount(transaction, 0.3);
    globalVolume->undoableSelectModSource(transaction, MacroControls::MC1);

    globalTune->setCPFromHwui(transaction, 0.60);
    globalTune->setModulationAmount(transaction, 0.5);
    globalTune->undoableSelectModSource(transaction, MacroControls::MC2);

    EBL::getPartVolume<VoiceGroup::II>()->setCPFromHwui(transaction, 0.75);
    EBL::getPartVolume<VoiceGroup::II>()->setModulationAmount(transaction, 0.4);
    EBL::getPartVolume<VoiceGroup::II>()->undoableSelectModSource(transaction, MacroControls::MC3);

    EBL::getPartTune<VoiceGroup::II>()->setCPFromHwui(transaction, 1);
    EBL::getPartTune<VoiceGroup::II>()->setModulationAmount(transaction, 0.3);
    EBL::getPartTune<VoiceGroup::II>()->undoableSelectModSource(transaction, MacroControls::MC5);

    CHECK(EBL::getPartTune<VoiceGroup::II>()->getModulationSource() == MacroControls::MC5);
    CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationSource() == MacroControls::MC3);

    envAIIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);

    detail::randomizeCrossFBAndToFX(transaction);
    detail::randomizeFadeParams(transaction);
  }

  WHEN("Converted")
  {

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto attackIICP = envAIIAttack->getControlPositionValue();

    const auto masterVolumeDisplay = globalVolume->getDisplayValue();
    const auto masterTuneDisplay = globalTune->getDisplayValue();

    const auto partTuneIIDisplay = EBL::getPartTune<VoiceGroup::II>()->getDisplayValue();
    const auto oldTuneIIModAmt = EBL::getPartTune<VoiceGroup::II>()->getModulationAmount();
    const auto oldTuneIIModSrc = EBL::getPartTune<VoiceGroup::II>()->getModulationSource();

    const auto partVolumeIIDisplay = EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue();
    const auto oldVolumeIIModAmt = EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount();
    const auto oldVolumeIIModSrc = EBL::getPartVolume<VoiceGroup::II>()->getModulationSource();

    const auto localNormalIIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();

    eb->undoableConvertToSingle(transaction, VoiceGroup::II);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
    }

    THEN("Special Local Params are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Local II copied to I")
    {
      //Check example Parameter
      CHECK_PARAMETER_CP_EQUALS_FICTION(envAIAttack, attackIICP);
      CHECK(localNormalIIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
    }

    THEN("Voices/Mono from II copied to I")
    {
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
      CHECK(EBL::getUnisonVoice<VoiceGroup::I>()->getDisplayString() == "12 voices");
    }

    THEN("Local II is default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getLocalNormal<VoiceGroup::II>()));
    }

    THEN("Voice Parameters of II are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getMono<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getUnison<VoiceGroup::II>()));
    }

    THEN("Part Master is Default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getPartTune<VoiceGroup::I>(), EBL::getPartTune<VoiceGroup::II>(),
                                   EBL::getPartVolume<VoiceGroup::I>(), EBL::getPartVolume<VoiceGroup::II>() }));
    }

    THEN("Split is default")
    {
      CHECK(EBL::getSplitPoint()->isDefaultLoaded());
    }

    THEN("Global Scale/MCM is unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("Part and Global master have been combined")
    {
      CHECK(globalVolume->getDisplayValue() == Approx(masterVolumeDisplay + partVolumeIIDisplay));
      CHECK(globalVolume->getModulationAmount() == oldVolumeIIModAmt);
      CHECK(globalVolume->getModulationSource() == oldVolumeIIModSrc);

      CHECK(globalTune->getDisplayValue() == Approx(std::min(masterTuneDisplay + partTuneIIDisplay, 48.0)));
      CHECK(globalTune->getModulationAmount() == oldTuneIIModAmt);
      CHECK(globalTune->getModulationSource() == oldTuneIIModSrc);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Convert Split (II) to Layer")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  auto attI = EBL::getParameter({ 0, VoiceGroup::I });
  auto attII = EBL::getParameter({ 0, VoiceGroup::II });

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initDualEditBuffer<SoundType::Split>(scope->getTransaction());
    voicesII->loadDefault(scope->getTransaction());
    voicesII->stepCPFromHwui(scope->getTransaction(), 12, {});
    CHECK(voicesII->getDisplayString() == "12 voices");

    monoII->setCPFromHwui(scope->getTransaction(), 1);
    CHECK(monoII->getDisplayString() == "On");

    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    for(auto p : EBL::getLocalNormal<VoiceGroup::I>())
      TestHelper::forceParameterChange(scope->getTransaction(), p);

    for(auto p : EBL::getLocalNormal<VoiceGroup::II>())
      TestHelper::forceParameterChange(scope->getTransaction(), p);

    for(auto p : EBL::getPartMaster<VoiceGroup::I>())
      TestHelper::forceParameterChange(scope->getTransaction(), p);

    for(auto p : EBL::getPartMaster<VoiceGroup::II>())
      TestHelper::forceParameterChange(scope->getTransaction(), p);

    for(auto p : EBL::getToFX<VoiceGroup::I>())
      TestHelper::forceParameterChange(scope->getTransaction(), p);

    for(auto p : EBL::getToFX<VoiceGroup::II>())
      TestHelper::forceParameterChange(scope->getTransaction(), p);

    attI->setCPFromHwui(scope->getTransaction(), 0.187);
    attII->setCPFromHwui(scope->getTransaction(), 0.287);
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>());
    const auto localIIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>());

    const auto splitCP = EBL::getSplitPoint()->getControlPositionValue();

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto masterVolumeHash = EBL::getMasterVolume()->getHash();
    const auto masterTuneHash = EBL::getMasterTune()->getHash();

    const auto oldToFxI = EBL::createValueHash(EBL::getToFX<VoiceGroup::I>());
    const auto oldToFxII = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());

    const auto oldPartMasterHashI = EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>());
    const auto oldPartMasterHashII = EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToDual(transaction, SoundType::Layer);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(voicesII->getDisplayString() == "1 voice (off)");
    }

    THEN("Cross FB I/II default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("to FX unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::I>()) == oldToFxI);
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::II>()) == oldToFxII);
    }

    THEN("Local normal unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()) == localIHash);
      CHECK(EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::II>()) == localIIHash);
      CHECK_PARAMETER_CP_EQUALS_FICTION(attI, 0.187);
      CHECK_PARAMETER_CP_EQUALS_FICTION(attII, 0.287);
    }

    THEN("Voices/Mono from II copied to I")
    {
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
      CHECK(EBL::getUnisonVoice<VoiceGroup::I>()->getDisplayString() == "12 voices");
    }

    THEN("Part Master is Unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>()) == oldPartMasterHashI);
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>()) == oldPartMasterHashII);
    }

    THEN("Voice Parameters of II are default")
    {
      for(auto& p : EBL::getMono<VoiceGroup::II>())
      {
        CHECK(p->isDefaultLoaded());
      }

      for(auto& p : EBL::getUnison<VoiceGroup::II>())
      {
        CHECK(p->isDefaultLoaded());
      }
    }

    THEN("Fade is determined from Split and Range is Default")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getFadeFrom<VoiceGroup::I>(), splitCP);
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getFadeFrom<VoiceGroup::II>(), splitCP);
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Split is default")
    {
      CHECK(EBL::getSplitPoint()->isDefaultLoaded());
    }

    THEN("Global Master/Scale/MCM is unchanged")
    {
      CHECK(EBL::getMasterVolume()->getHash() == masterVolumeHash);
      CHECK(EBL::getMasterTune()->getHash() == masterTuneHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Convert Layer I to Split")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();
  auto globalVolume = EBL::getMasterVolume();
  auto globalTune = EBL::getMasterVolume();

  auto envAIAttack = EBL::getParameter({ 0, VoiceGroup::I });
  auto envAIIAttack = EBL::getParameter({ 0, VoiceGroup::II });

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);

    voicesI->loadDefault(transaction);
    voicesI->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesI->getDisplayString() == "12 voices");

    monoI->setCPFromHwui(transaction, 1);
    CHECK(monoI->getDisplayString() == "On");

    globalVolume->setModulationAmount(transaction, 0.5);
    globalVolume->undoableSelectModSource(transaction, MacroControls::MC1);

    globalTune->setModulationAmount(transaction, 0.5);
    globalTune->undoableSelectModSource(transaction, MacroControls::MC2);

    EBL::getFadeFrom<VoiceGroup::I>()->setCPFromHwui(transaction, 0.187);
    EBL::getFadeFrom<VoiceGroup::II>()->setCPFromHwui(transaction, 0.887);

    EBL::getPartVolume<VoiceGroup::I>()->setCPFromHwui(transaction, 0.25);
    EBL::getPartVolume<VoiceGroup::I>()->setModulationAmount(transaction, 0.4);
    EBL::getPartVolume<VoiceGroup::I>()->undoableSelectModSource(transaction, MacroControls::MC3);

    EBL::getPartTune<VoiceGroup::I>()->setCPFromHwui(transaction, 0.25);
    EBL::getPartTune<VoiceGroup::I>()->setModulationAmount(transaction, 0.4);
    EBL::getPartTune<VoiceGroup::I>()->undoableSelectModSource(transaction, MacroControls::MC5);

    envAIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);
    envAIIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);

    EBL::getFadeFrom<VoiceGroup::I>()->setCPFromHwui(transaction, 0.7);
    EBL::getFadeFrom<VoiceGroup::II>()->setCPFromHwui(transaction, 0.7);

    detail::randomizeCrossFBAndToFX(transaction);
  }

  WHEN("Converted")
  {
    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto attackICP = envAIAttack->getControlPositionValue();
    const auto attackIICP = envAIIAttack->getControlPositionValue();

    const auto masterVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto masterTuneDisplay = EBL::getMasterVolume()->getDisplayValue();

    const auto oldFromI = EBL::getFadeFrom<VoiceGroup::I>()->getControlPositionValue();
    const auto oldFromII = EBL::getFadeFrom<VoiceGroup::II>()->getControlPositionValue();

    const auto oldLocalIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());
    const auto oldLocalIIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>());

    const auto partIMasterHash = EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>());
    const auto partIIMasterHash = EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>());

    const auto oldToFXIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::I>());
    const auto oldToFXIIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToDual(transaction, SoundType::Split);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");

      CHECK(EBL::getUnisonVoice<VoiceGroup::II>()->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::II>()->getDisplayString() == "On");
    }

    THEN("CrossFB Params are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("To FX Unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::I>()) == oldToFXIHash);
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::II>()) == oldToFXIIHash);
    }

    THEN("Local kept values")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(envAIAttack, attackICP);
      CHECK_PARAMETER_CP_EQUALS_FICTION(envAIIAttack, attackIICP);

      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>()) == oldLocalIHash);
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>()) == oldLocalIIHash);
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Part Master was copied from I")
    {
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>()) == partIMasterHash);
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>()) == partIIMasterHash);
    }

    THEN("Split is mean of fade from I and II")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getSplitPoint(), (oldFromI + oldFromII) / 2.0);
    }

    THEN("Global Scale/MCM is unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("Global master did not changed")
    {
      CHECK(EBL::getMasterVolume()->getDisplayValue() == masterVolumeDisplay);
      CHECK(EBL::getMasterTune()->getDisplayValue() == masterTuneDisplay);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Convert Layer (II) to Single")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();

  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  auto globalVolume = EBL::getMasterVolume();
  auto globalTune = EBL::getMasterVolume();

  auto envAIAttack = EBL::getParameter({ 0, VoiceGroup::I });
  auto envAIIAttack = EBL::getParameter({ 0, VoiceGroup::II });

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);

    voicesII->loadDefault(transaction);
    voicesII->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesII->getDisplayString() == "12 voices");

    monoII->setCPFromHwui(transaction, 1);
    CHECK(monoII->getDisplayString() == "On");

    globalTune->setCPFromHwui(transaction, 0.25);
    globalVolume->setModulationAmount(transaction, 0.5);
    globalVolume->undoableSelectModSource(transaction, MacroControls::MC1);

    globalTune->setCPFromHwui(transaction, 0.5);
    globalTune->setModulationAmount(transaction, 0.5);
    globalTune->undoableSelectModSource(transaction, MacroControls::MC2);

    EBL::getPartVolume<VoiceGroup::II>()->setModulationAmount(transaction, 0.4);
    EBL::getPartVolume<VoiceGroup::II>()->undoableSelectModSource(transaction, MacroControls::MC3);
    EBL::getPartTune<VoiceGroup::II>()->setModulationAmount(transaction, 0.4);
    EBL::getPartTune<VoiceGroup::II>()->undoableSelectModSource(transaction, MacroControls::MC5);

    envAIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);
    envAIIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);

    detail::randomizeCrossFBAndToFX(transaction);
    detail::randomizeFadeParams(transaction);
  }

  WHEN("Converted")
  {
    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto oldAttackIICP = envAIIAttack->getControlPositionValue();

    const auto oldMasterVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldMasterTuneDisplay = EBL::getMasterVolume()->getDisplayValue();

    const auto oldPartTuneIIDisplay = EBL::getPartTune<VoiceGroup::II>()->getDisplayValue();
    const auto oldPartVolumeIIDisplay = EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue();

    const auto oldPartTuneModSrc = EBL::getPartTune<VoiceGroup::II>()->getModulationSource();
    const auto oldPartTuneModAmt = EBL::getPartTune<VoiceGroup::II>()->getModulationAmount();

    const auto oldPartVolumeModSrc = EBL::getPartVolume<VoiceGroup::II>()->getModulationSource();
    const auto oldPartVolumeModAmt = EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount();

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();

    auto selectedVG = Application::get().getHWUI()->getCurrentVoiceGroup();

    eb->undoableConvertToSingle(transaction, selectedVG);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");

      CHECK(EBL::getUnisonVoice<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getMonoEnable<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Special Local Params are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>(), EBL::getCrossFB<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getToFX<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("Local I has Local II and Local II is default")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(envAIAttack, oldAttackIICP);
      CHECK(EBL::isDefaultLoaded(EBL::getLocalNormal<VoiceGroup::II>()));
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Part Master was copied from I")
    {
      CHECK(EBL::getPartTune<VoiceGroup::I>()->getDisplayValue() == oldPartTuneIIDisplay + oldMasterTuneDisplay);
      CHECK(EBL::getPartVolume<VoiceGroup::I>()->getDisplayValue() == oldPartVolumeIIDisplay + oldMasterVolumeDisplay);

      CHECK(EBL::getMasterVolume()->getModulationAmount() == oldPartVolumeModAmt);
      CHECK(EBL::getMasterTune()->getModulationAmount() == oldPartTuneModAmt);

      CHECK(EBL::getMasterVolume()->getModulationSource() == oldPartVolumeModSrc);
      CHECK(EBL::getMasterTune()->getModulationSource() == oldPartTuneModSrc);

      CHECK(EBL::isDefaultLoaded(EBL::getPartMaster<VoiceGroup::I>(), EBL::getPartMaster<VoiceGroup::II>()));
    }

    THEN("Split is default")
    {
      CHECK(EBL::getSplitPoint()->isDefaultLoaded());
    }

    THEN("Global Scale/MCM is unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

/*
 * LOADING TESTS
 * LOADING BELOW
 */

TEST_CASE("Load Single into Split Part II")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getSinglePreset();

  REQUIRE(preset->getType() == SoundType::Single);

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);
    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    auto envAAttack = preset->findParameterByID({ 0, VoiceGroup::I }, true);
    envAAttack->setValue(transaction, 0.666);

    auto volume = preset->findParameterByID({ 247, VoiceGroup::Global }, true);
    volume->setValue(transaction, 0.265);
    volume->setField(transaction, PresetParameter::Fields::ModSource, "1");
    volume->setField(transaction, PresetParameter::Fields::ModAmount, "1");

    auto unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true);
    unisonVoices->setValue(transaction, 1);  // <- setting cp in range 0..23

    detail::changeAllParameters(transaction);
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
        = EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash();

    const auto oldUnisonIHash = EBL::createValueHash(EBL::getUnison<VoiceGroup::I>(), EBL::getMono<VoiceGroup::I>());

    const auto oldFadeIIHash
        = EBL::createValueHash({ EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>() });

    eb->undoableLoadSinglePreset(preset, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Local Special unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>()) == localSpecialIHash);
      CHECK(EBL::createValueHash(EBL::getToFX<VoiceGroup::II>()) == toFXIIHash);
    }

    THEN("Cross FB loaded to is Default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono Loaded from preset")
    {
      CHECK(oldUnisonIHash == EBL::createValueHash(EBL::getUnison<VoiceGroup::I>(), EBL::getMono<VoiceGroup::I>()));
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getUnisonVoice<VoiceGroup::II>(), 1);  //compare cp for 0..11
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

    THEN("Fade I is untouched")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash()
            == oldFadeIHash);
    }

    THEN("Fade II is ignored")
    {
      CHECK(EBL::createValueHash({ EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>() })
            == oldFadeIIHash);
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

    detail::changeAllParameters(transaction);
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
  }

  WHEN("Load")
  {
    const auto localSpecialIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>());
    const auto localSpecialIIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>(), EBL::getToFX<VoiceGroup::II>());
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

    const auto oldToFXIIHash = EBL::createValueHash(EBL::getToFX<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Local Special I unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>()) == localSpecialIHash);
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

    THEN("Local I untouched")
    {
      CHECK(localNormalIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
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
            == (presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue()
            == (presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay);

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationSource() == MacroControls::MC1);
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount() == 1);
    }

    THEN("Global Groups unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getMaster()) == oldMasterHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == oldMCMHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == oldScaleHash);
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

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Load Part I of Split into Layer Part II")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getSplitPreset();

  REQUIRE(preset->getType() == SoundType::Split);

  REQUIRE_NOTHROW(preset->findParameterByID({ 356, VoiceGroup::Global }, true));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
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

    for(auto p : EBL::getToFX<VoiceGroup::I>())
    {
      auto toFX = preset->findParameterByID({ p->getID().getNumber(), VoiceGroup::I }, true);
      toFX->setValue(transaction, 0.187);
    }
  }

  WHEN("Load")
  {
    const auto localSpecialIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>());
    const auto localSpecialIIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>(), EBL::getToFX<VoiceGroup::II>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto localNormalIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>());

    const auto oldFadeIHash
        = EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash();
    const auto oldFadeIIHash
        = EBL::getFadeFrom<VoiceGroup::II>()->getHash() + EBL::getFadeRange<VoiceGroup::II>()->getHash();

    const auto oldUnisonMonoIHash
        = EBL::createValueHash(EBL::getMono<VoiceGroup::I>(), EBL::getUnison<VoiceGroup::I>());

    const auto oldCrossFBIIHash = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Layer);
    }

    THEN("ToFX II copied from Preset")
    {
      for(auto& p : EBL::getToFX<VoiceGroup::II>())
      {
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.187);
      }
    }

    THEN("Cross FB II ignored")
    {
      CHECK(oldCrossFBIIHash == EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Local I untouched")
    {
      CHECK(localNormalIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono II are default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getUnison<VoiceGroup::II>()));
      CHECK(EBL::isDefaultLoaded(EBL::getMono<VoiceGroup::II>()));
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
            == (presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue()
            == (presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay);

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationSource() == MacroControls::MC1);
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount() == 1);
    }

    THEN("Global Groups unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getMaster()) == oldMasterHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == oldMCMHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == oldScaleHash);
    }

    THEN("Fade I/II untouched")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash()
            == oldFadeIHash);
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->getHash() + EBL::getFadeRange<VoiceGroup::II>()->getHash()
            == oldFadeIIHash);
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
  }

  WHEN("Load")
  {
    const auto localSpecialIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>());
    const auto localSpecialIIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::II>(), EBL::getToFX<VoiceGroup::II>());
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

    eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Local Special I unchanged")
    {
      CHECK(EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>()) == localSpecialIHash);
    }

    THEN("ToFX II copied from Preset")
    {
      for(auto& p : EBL::getToFX<VoiceGroup::II>())
      {
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.187);
      }
    }

    THEN("Cross FB II default")
    {
      CHECK(EBL::isDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Local I untouched")
    {
      CHECK(localNormalIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
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
            == (presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue()
            == (presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay);

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationSource() == MacroControls::MC1);
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount() == 1);
    }

    THEN("Global Groups unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getMaster()) == oldMasterHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == oldMCMHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == oldScaleHash);
    }

    THEN("Fade II default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getFadeFrom<VoiceGroup::II>(), EBL::getFadeRange<VoiceGroup::II>() }));
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}

TEST_CASE("Load Part I of Layer into Layer Part II")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;
  auto preset = presets.getLayerPreset();

  REQUIRE(preset->getType() == SoundType::Layer);

  REQUIRE_NOTHROW(preset->findParameterByID({ 356, VoiceGroup::Global }, true));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
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

    for(auto p : EBL::getCrossFB<VoiceGroup::I>())
    {
      auto crossFB = preset->findParameterByID({ p->getID().getNumber(), VoiceGroup::I }, true);
      crossFB->setValue(transaction, 0.187);
    }
  }

  WHEN("Load")
  {
    const auto localSpecialIHash
        = EBL::createValueHash(EBL::getCrossFB<VoiceGroup::I>(), EBL::getToFX<VoiceGroup::I>());

    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto localNormalIHash = EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>());

    const auto oldUnisonMonoIHash
        = EBL::createValueHash(EBL::getMono<VoiceGroup::I>(), EBL::getUnison<VoiceGroup::I>());

    const auto oldFadeHash
        = EBL::createValueHash({ EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeFrom<VoiceGroup::II>(),
                                 EBL::getFadeRange<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::II>() });

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Layer);
    }

    THEN("ToFX II copied from Preset")
    {
      for(auto& p : EBL::getToFX<VoiceGroup::II>())
      {
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.187);
      }
    }

    THEN("Cross FB II copied from Preset")
    {
      for(auto& p : EBL::getCrossFB<VoiceGroup::II>())
      {
        CHECK_PARAMETER_CP_EQUALS_FICTION(p, 0.187);
      }
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Local I untouched")
    {
      CHECK(localNormalIHash == EBL::createValueHash(EBL::getLocalNormal<VoiceGroup::I>()));
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

    THEN("Unison and Mono II are default")
    {
      CHECK(EBL::isDefaultLoaded({ EBL::getUnisonVoice<VoiceGroup::II>(), EBL::getMonoEnable<VoiceGroup::II>() }));
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
            == (presetGlobalVolumeDisplay + presetPartVolumeDisplay) - oldVolumeDisplay);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      auto presetPartTuneDisplay = tuneScale->controlPositionToDisplay(ogPartTune->getValue());

      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue()
            == (presetGlobalTuneDisplay + presetPartTuneDisplay) - oldTuneDisplay);

      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationSource() == MacroControls::MC1);
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount() == 1);
    }

    THEN("Global Groups unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getMaster()) == oldMasterHash);
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == oldMCMHash);
      CHECK(EBL::createHashOfVector(EBL::getScale()) == oldScaleHash);
    }

    THEN("Fade I/II ignored")
    {
      CHECK(oldFadeHash
            == EBL::createValueHash({ EBL::getFadeFrom<VoiceGroup::I>(), EBL::getFadeFrom<VoiceGroup::II>(),
                                      EBL::getFadeRange<VoiceGroup::I>(), EBL::getFadeRange<VoiceGroup::II>() }));
    }

    THEN("EB unchanged")
    {
      CHECK_FALSE(eb->findAnyParameterChanged());
    }
  }
}