#include <iostream>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "testing/TestHelper.h"
#include "testing/unit-tests/mock/EditBufferNamedLogicalParts.h"
#include "proxies/hwui/HWUI.h"
#include "presets/Preset.h"
#include "presets/PresetParameter.h"
#include "parameters/SplitPointParameter.h"
#include "parameters/scale-converters/ScaleConverter.h"

using EBL = EditBufferLogicalParts;

TEST_CASE("Convert Single Sound to Split")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initSingleEditBuffer(scope->getTransaction());
    voicesI->loadDefault(scope->getTransaction());
    voicesI->stepCPFromHwui(scope->getTransaction(), 12, {});
    CHECK(voicesI->getDisplayString() == "13 voices");

    monoI->setCPFromHwui(scope->getTransaction(), 1);
    CHECK(monoI->getDisplayString() == "On");
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());
    const auto localIMono = EBL::createHashOfVector(EBL::getMono<VoiceGroup::I>());

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

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
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::I>())
        CHECK(special->isDefaultLoaded());
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::II>())
        CHECK(special->isDefaultLoaded());
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

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
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

    THEN("Macro Mappings are same")
    {
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("Scale unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
    }
  }
}

TEST_CASE("Convert Single Sound to Layer")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initSingleEditBuffer(scope->getTransaction());
    voicesI->loadDefault(scope->getTransaction());
    voicesI->stepCPFromHwui(scope->getTransaction(), 12, {});
    CHECK(voicesI->getDisplayString() == "13 voices");

    monoI->setCPFromHwui(scope->getTransaction(), 1);
    CHECK(monoI->getDisplayString() == "On");
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToDual(transaction, SoundType::Layer);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(voicesII->getDisplayString() == "1 voice (off)");
    }

    THEN("Special Local Params are default")
    {
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::I>())
        CHECK(special->isDefaultLoaded());
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::II>())
        CHECK(special->isDefaultLoaded());
    }

    THEN("Local normal I have been copied to II")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>()) == localIHash);
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

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
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

    THEN("Macro Mappings are same")
    {
      CHECK(EBL::createHashOfVector(EBL::getModMatrix()) == mcmHash);
    }

    THEN("Scale unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getScale()) == scaleHash);
    }
  }
}

TEST_CASE("Convert Split (II) to Layer")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initDualEditBuffer<SoundType::Split>(scope->getTransaction());
    voicesII->loadDefault(scope->getTransaction());
    voicesII->stepCPFromHwui(scope->getTransaction(), 12, {});
    CHECK(voicesII->getDisplayString() == "12 voices");

    monoII->setCPFromHwui(scope->getTransaction(), 1);
    CHECK(monoII->getDisplayString() == "On");

    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());
    const auto localIIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>());

    const auto splitCP = EBL::getSplitPoint()->getControlPositionValue();

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto masterVolumeHash = EBL::getMasterVolume()->getHash();
    const auto masterTuneHash = EBL::getMasterTune()->getHash();

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToDual(transaction, SoundType::Layer);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(voicesII->getDisplayString() == "1 voice (off)");
    }

    THEN("Special Local Params are default")
    {
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::I>())
        CHECK(special->isDefaultLoaded());
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::II>())
        CHECK(special->isDefaultLoaded());
    }

    THEN("Local normal unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>()) == localIHash);
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>()) == localIIHash);
    }

    THEN("Voices/Mono from II copied to I")
    {
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
      CHECK(EBL::getUnisonVoice<VoiceGroup::I>()->getDisplayString() == "12 voices");
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

    THEN("Fade is determined from Split")
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
  }
}

TEST_CASE("Convert Split (II) to Single")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();
  auto globalVolume = EBL::getMasterVolume();
  auto globalTune = EBL::getMasterVolume();

  auto envAIIAttack = EBL::getParameter({ 0, VoiceGroup::II });
  auto envAIAttack = EBL::getParameter({ 0, VoiceGroup::I });

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);

    voicesII->loadDefault(transaction);
    voicesII->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesII->getDisplayString() == "12 voices");

    monoII->setCPFromHwui(transaction, 1);
    CHECK(monoII->getDisplayString() == "On");

    globalVolume->setModulationAmount(transaction, 0.5);
    globalVolume->undoableSelectModSource(transaction, MacroControls::MC1);

    globalTune->setModulationAmount(transaction, 0.5);
    globalTune->undoableSelectModSource(transaction, MacroControls::MC2);

    EBL::getPartVolume<VoiceGroup::II>()->setModulationAmount(transaction, 0.4);
    EBL::getPartVolume<VoiceGroup::II>()->undoableSelectModSource(transaction, MacroControls::MC3);
    EBL::getPartTune<VoiceGroup::II>()->setModulationAmount(transaction, 0.4);
    EBL::getPartTune<VoiceGroup::II>()->undoableSelectModSource(transaction, MacroControls::MC5);

    Application::get().getHWUI()->setCurrentVoiceGroup(VoiceGroup::II);

    envAIIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);
  }

  WHEN("Converted")
  {
    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto attackIICP = envAIIAttack->getControlPositionValue();

    const auto masterVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto masterTuneDisplay = EBL::getMasterVolume()->getDisplayValue();

    const auto partVolumeIIDisplay = EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue();
    const auto partTuneIIDisplay = EBL::getPartTune<VoiceGroup::II>()->getDisplayValue();

    const auto oldTuneIIModAmt = EBL::getPartTune<VoiceGroup::II>()->getModulationAmount();
    const auto oldTuneIIModSrc = EBL::getPartTune<VoiceGroup::II>()->getModulationSource();

    const auto oldVolumeIIModAmt = EBL::getPartVolume<VoiceGroup::II>()->getModulationAmount();
    const auto oldVolumeIIModSrc = EBL::getPartVolume<VoiceGroup::II>()->getModulationSource();

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    auto eb = TestHelper::getEditBuffer();
    eb->undoableConvertToSingle(transaction, Application::get().getHWUI()->getCurrentVoiceGroup());

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
    }

    THEN("Special Local Params are default")
    {
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::I>())
        CHECK(special->isDefaultLoaded());
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::II>())
        CHECK(special->isDefaultLoaded());
    }

    THEN("Local II copied to I")
    {
      //Check example Parameter
      CHECK_PARAMETER_CP_EQUALS_FICTION(envAIAttack, attackIICP);
    }

    THEN("Voices/Mono from II copied to I")
    {
      CHECK(EBL::getMonoEnable<VoiceGroup::I>()->getDisplayString() == "On");
      CHECK(EBL::getUnisonVoice<VoiceGroup::I>()->getDisplayString() == "12 voices");
    }

    THEN("Local II is default")
    {
      for(auto& p : EBL::getLocalNormal<VoiceGroup::II>())
      {
        CHECK(p->isDefaultLoaded());
      }

      for(auto& p : EBL::getLocalSpecial<VoiceGroup::II>())
      {
        CHECK(p->isDefaultLoaded());
      }
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

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isDefaultLoaded());
    }

    THEN("Part Master is Default")
    {
      CHECK(EBL::getPartTune<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getPartTune<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getPartVolume<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->isDefaultLoaded());
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
      CHECK(EBL::getMasterVolume()->getDisplayValue() == masterVolumeDisplay + partVolumeIIDisplay);
      CHECK(EBL::getMasterVolume()->getModulationAmount() == oldVolumeIIModAmt);
      CHECK(EBL::getMasterVolume()->getModulationSource() == oldVolumeIIModSrc);

      CHECK(EBL::getMasterTune()->getDisplayValue() == masterTuneDisplay + partTuneIIDisplay);
      CHECK(EBL::getMasterTune()->getModulationAmount() == oldTuneIIModAmt);
      CHECK(EBL::getMasterTune()->getModulationSource() == oldTuneIIModSrc);
    }
  }
}

TEST_CASE("Convert Layer to Split")
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

    EBL::getPartVolume<VoiceGroup::I>()->setModulationAmount(transaction, 0.4);
    EBL::getPartVolume<VoiceGroup::I>()->undoableSelectModSource(transaction, MacroControls::MC3);
    EBL::getPartTune<VoiceGroup::I>()->setModulationAmount(transaction, 0.4);
    EBL::getPartTune<VoiceGroup::I>()->undoableSelectModSource(transaction, MacroControls::MC5);

    envAIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);
    envAIIAttack->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 0.4);
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

    const auto oldPartTuneIDisplay = EBL::getPartTune<VoiceGroup::I>()->getDisplayValue();
    const auto oldPartVolumeIDisplay = EBL::getPartVolume<VoiceGroup::I>()->getDisplayValue();

    const auto oldLocalIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());
    const auto oldLocalIIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>());

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

    THEN("Special Local Params are default")
    {
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::I>())
        CHECK(special->isDefaultLoaded());
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::II>())
        CHECK(special->isDefaultLoaded());
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
      CHECK(EBL::getPartTune<VoiceGroup::I>()->getDisplayValue() == oldPartTuneIDisplay);
      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue() == oldPartTuneIDisplay);
      CHECK(EBL::getPartVolume<VoiceGroup::I>()->getDisplayValue() == oldPartVolumeIDisplay);
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue() == oldPartVolumeIDisplay);
    }

    THEN("Split is mean of fade from I and II")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(EBL::getSplitPoint(), (oldFromI + oldFromII) / 2);
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
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::I>())
        CHECK(special->isDefaultLoaded());
      for(auto& special : EBL::getLocalSpecial<VoiceGroup::II>())
        CHECK(special->isDefaultLoaded());
    }

    THEN("Local I has Local II and Local II is default")
    {
      //check for copy of VG II params to VG I Params
      CHECK_PARAMETER_CP_EQUALS_FICTION(envAIAttack, oldAttackIICP);

      for(auto& p : EBL::getLocalNormal<VoiceGroup::II>())
      {
        CHECK(p->isDefaultLoaded());
      }
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

      CHECK(EBL::getPartVolume<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getPartTune<VoiceGroup::I>()->isDefaultLoaded());
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->isDefaultLoaded());
      CHECK(EBL::getPartTune<VoiceGroup::II>()->isDefaultLoaded());
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
  }
}

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
  }

  WHEN("Load")
  {
    const auto localSpecialIHash = EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::I>());
    const auto localSpecialIIHash = EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::II>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldUnisonIIHash = EBL::createHashOfVector(EBL::getUnison<VoiceGroup::II>());
    const auto oldMonoIIHash = EBL::createHashOfVector(EBL::getMono<VoiceGroup::II>());

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto oldFadeIHash
        = EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash();

    const auto oldFadeIIHash
        = EBL::getFadeFrom<VoiceGroup::II>()->getHash() + EBL::getFadeRange<VoiceGroup::II>()->getHash();

    eb->undoableLoadSinglePreset(preset, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Local Special unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::I>()) == localSpecialIHash);
      CHECK(EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::II>()) == localSpecialIIHash);
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

#warning "I think this is wrong -> unison and mono should be copied"
#warning TODO
    THEN("Unison and Mono unchanged")
    {
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

      auto presetGlobalVolumeDisplay = volumeScale->controlPositionToDisplay(vol->getValue());
      CHECK(EBL::getPartVolume<VoiceGroup::II>()->getDisplayValue() == presetGlobalVolumeDisplay - oldVolumeDisplay);

      auto presetGlobalTuneDisplay = tuneScale->controlPositionToDisplay(tune->getValue());
      CHECK(EBL::getPartTune<VoiceGroup::II>()->getDisplayValue() == presetGlobalTuneDisplay - oldTuneDisplay);

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
  }
}

//Continue here
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
  }

  WHEN("Load")
  {
    const auto localSpecialIHash = EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::I>());
    const auto localSpecialIIHash = EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::II>());
    const auto oldSplitCP = eb->getSplitPoint()->getControlPositionValue();

    const auto oldUnisonIIHash = EBL::createHashOfVector(EBL::getUnison<VoiceGroup::II>());
    const auto oldMonoIIHash = EBL::createHashOfVector(EBL::getMono<VoiceGroup::II>());

    const auto oldVolumeDisplay = EBL::getMasterVolume()->getDisplayValue();
    const auto oldTuneDisplay = EBL::getMasterTune()->getDisplayValue();

    const auto oldMasterHash = EBL::createHashOfVector(EBL::getMaster());
    const auto oldScaleHash = EBL::createHashOfVector(EBL::getScale());
    const auto oldMCMHash = EBL::createHashOfVector(EBL::getModMatrix());

    const auto oldFadeIHash
        = EBL::getFadeFrom<VoiceGroup::I>()->getHash() + EBL::getFadeRange<VoiceGroup::I>()->getHash();

    const auto oldFadeIIHash
        = EBL::getFadeFrom<VoiceGroup::II>()->getHash() + EBL::getFadeRange<VoiceGroup::II>()->getHash();

    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::II);

    THEN("Type is Same")
    {
      CHECK(eb->getType() == SoundType::Split);
    }

    THEN("Local Special unchanged")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::I>()) == localSpecialIHash);
      CHECK(EBL::createHashOfVector(EBL::getLocalSpecial<VoiceGroup::II>()) == localSpecialIIHash);
    }

    THEN("Local Normal was copied to current VG")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->findParameterByID({ 0, VoiceGroup::II }), 0.666);
    }

    THEN("Split unchanged")
    {
      CHECK_PARAMETER_CP_EQUALS_FICTION(eb->getSplitPoint(), oldSplitCP);
    }

#warning "I think this is wrong -> unison and mono should be copied"
#warning TODO
    THEN("Unison and Mono unchanged")
    {
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
  }
}