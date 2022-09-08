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

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Convert Single Sound to Split")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();
  auto monoII = EBL::getMonoEnable<VoiceGroup::II>();

  auto globalVolume = EBL::getMasterVolume();

  {
    TestHelper::initSingleEditBuffer();
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();
    voicesI->loadDefault(transaction, Defaults::FactoryDefault);
    voicesI->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesI->getDisplayString() == "13 voices");

    monoI->setCPFromHwui(transaction, 1);
    CHECK(monoI->getDisplayString() == "On");

    globalVolume->setModulationAmount(transaction, 0.45);
    globalVolume->setModulationSource(transaction, MacroControls::MC2);
    globalVolume->setCPFromHwui(transaction, 0.187);

    TestHelper::randomizeCrossFBAndToFX(transaction);
    TestHelper::randomizeFadeParams(transaction);
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());
    const auto localIMono = EBL::createHashOfVector(EBL::getMono<VoiceGroup::I>());

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto globalMasterHash = EBL::createValueHash(EBL::getMaster());

    auto eb = TestHelper::getEditBuffer();
    EditBufferUseCases useCase(*eb);
    useCase.convertToSplit(VoiceGroup::I);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
      CHECK(voicesII->getDisplayString() == "12 voices");
    }

    THEN("Special Local Params are default")
    {
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getToFX<VoiceGroup::I>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isFactoryDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isFactoryDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isFactoryDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isFactoryDefaultLoaded());
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
      CHECK(EBL::getSplitPoint<VoiceGroup::I>()->isFactoryDefaultLoaded());
      CHECK(EBL::getSplitPoint<VoiceGroup::II>()->isFactoryDefaultLoaded());
    }

    THEN("Global Master is Default")
    {
      CHECK(EBL::getMasterVolume()->isFactoryDefaultLoaded());
      CHECK(EBL::getMasterTune()->isFactoryDefaultLoaded());
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

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Convert Single Sound to Layer")
{
  auto voicesI = EBL::getUnisonVoice<VoiceGroup::I>();
  auto voicesII = EBL::getUnisonVoice<VoiceGroup::II>();
  auto monoI = EBL::getMonoEnable<VoiceGroup::I>();
  auto globalVolume = EBL::getMasterVolume();

  {
    TestHelper::initSingleEditBuffer();
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    voicesI->loadDefault(transaction, Defaults::FactoryDefault);
    voicesI->stepCPFromHwui(transaction, 12, {});
    CHECK(voicesI->getDisplayString() == "13 voices");

    monoI->setCPFromHwui(transaction, 1);
    CHECK(monoI->getDisplayString() == "On");

    globalVolume->setModulationAmount(transaction, 0.45);
    globalVolume->setModulationSource(transaction, MacroControls::MC2);
    globalVolume->setCPFromHwui(transaction, 0.187);

    TestHelper::randomizeCrossFBAndToFX(transaction);
    TestHelper::randomizeFadeParams(transaction);
  }

  WHEN("Converted")
  {
    const auto localIHash = EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::I>());

    const auto mcmHash = EBL::createHashOfVector(EBL::getModMatrix());
    const auto scaleHash = EBL::createHashOfVector(EBL::getScale());

    const auto oldMasterHash = EBL::createValueHash(EBL::getMaster());


    auto eb = TestHelper::getEditBuffer();
    EditBufferUseCases useCase(*eb);
    useCase.convertToLayer(VoiceGroup::I);

    THEN("Unison Voices Correct")
    {
      CHECK(voicesI->getDisplayString() == "12 voices");
    }

    THEN("Special Local Params are default")
    {
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getCrossFB<VoiceGroup::I>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getCrossFB<VoiceGroup::II>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getToFX<VoiceGroup::I>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getToFX<VoiceGroup::II>()));
    }

    THEN("Fade is default")
    {
      CHECK(EBL::getFadeFrom<VoiceGroup::I>()->isFactoryDefaultLoaded());
      CHECK(EBL::getFadeFrom<VoiceGroup::II>()->isFactoryDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::I>()->isFactoryDefaultLoaded());
      CHECK(EBL::getFadeRange<VoiceGroup::II>()->isFactoryDefaultLoaded());
    }

    THEN("Local normal I have been copied to II")
    {
      CHECK(EBL::createHashOfVector(EBL::getLocalNormal<VoiceGroup::II>()) == localIHash);
    }

    THEN("Voice Parameters of II are default")
    {
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getMono<VoiceGroup::II>()));
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getUnison<VoiceGroup::II>()));
    }

    THEN("Part Master I/II was Copied from Global Master")
    {
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::I>()) == oldMasterHash);
      CHECK(EBL::createValueHash(EBL::getPartMaster<VoiceGroup::II>()) == oldMasterHash);
    }

    THEN("Split is default")
    {
      CHECK(EBL::getSplitPoint<VoiceGroup::I>()->isFactoryDefaultLoaded());
      CHECK(EBL::getSplitPoint<VoiceGroup::II>()->isFactoryDefaultLoaded());
    }

    THEN("Global Master is Default")
    {
      CHECK(EBL::isFactoryDefaultLoaded(EBL::getMaster()));
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
