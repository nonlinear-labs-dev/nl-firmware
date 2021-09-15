#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include "testing/parameter/TestGroupSet.h"
#include "testing/parameter/TestGroup.h"
#include "testing/TestRootDocument.h"
#include "parameters/SplitPointParameter.h"
#include "testing/TestHelper.h"
#include "device-settings/Settings.h"
#include "device-settings/SplitPointSyncParameters.h"
#include <catch.hpp>
#include <parameter_declarations.h>
#include <device-settings/SyncSplitSettingUseCases.h>

TEST_CASE("Split Point Display Value")
{
  TestRootDocument root;
  TestGroupSet set { &root };
  TestGroup group(&set, VoiceGroup::I);
  group.addParameter(new SplitPointParameter(&group, ParameterId { 1, VoiceGroup::I }));
  group.addParameter(new SplitPointParameter(&group, ParameterId { 1, VoiceGroup::II }));

  auto splitI = dynamic_cast<SplitPointParameter*>(group.findParameterByID({ 1, VoiceGroup::I }));
  auto splitII = dynamic_cast<SplitPointParameter*>(group.findParameterByID({ 1, VoiceGroup::II }));
  g_assert(splitI != nullptr);

  auto settings = TestHelper::getSettings();
  auto pm = TestHelper::getPresetManager();

  SyncSplitSettingUseCases useCases(*settings->getSetting<SplitPointSyncParameters>(), *pm);

  auto transScope = UNDO::Scope::startTrashTransaction();
  auto transaction = transScope->getTransaction();

  auto syncSetting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  useCases.disableSyncSetting(transaction);

  splitI->setCPFromHwui(transaction, 0);
  splitII->setCPFromHwui(transaction, 0);
  CHECK(splitI->getDisplayString() == "C1");
  CHECK(splitII->getDisplayString() == "C1");

  splitI->setCPFromHwui(transaction, 0.5);
  splitII->setCPFromHwui(transaction, 0.5);
  CHECK(splitI->getDisplayString() == "F#3");
  CHECK(splitII->getDisplayString() == "F#3");

  splitI->setCPFromHwui(transaction, 1);
  splitII->setCPFromHwui(transaction, 1);
  CHECK(splitI->getDisplayString() == "C6");
  CHECK(splitII->getDisplayString() == "C6");
}

TEST_CASE("Split Point Helper Functions")
{
  auto eb = TestHelper::getEditBuffer();
  auto settings = TestHelper::getSettings();
  auto pm = TestHelper::getPresetManager();
  auto sI = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto sII = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::II });
  SyncSplitSettingUseCases useCases(*settings->getSetting<SplitPointSyncParameters>(), *pm);

  WHEN("Init")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    THEN("Default Split Behaviour I|II")
    {
      CHECK(sI->inDefaultSplitBehaviour());
      CHECK(sII->inDefaultSplitBehaviour());
    }

    WHEN("Sync Off and Overlap set")
    {
      auto scope = TestHelper::createTestScope();
      auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
      useCases.disableSyncSetting(scope->getTransaction());
      sI->stepCPFromHwui(scope->getTransaction(), 2, {});
      THEN("Split Behaviour has Overlap and not Default")
      {
        CHECK(sI->hasOverlap());
        CHECK(sII->hasOverlap());

        CHECK_FALSE(sI->inDefaultSplitBehaviour());
        CHECK_FALSE(sII->inDefaultSplitBehaviour());
      }
    }
  }
}

TEST_CASE("Sync Setting gets updated on store and load")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  auto settings = TestHelper::getSettings();
  EditBufferUseCases ebUseCases(*eb);

  auto sI = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto sII = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::II });
  SyncSplitSettingUseCases useCases(*settings->getSetting<SplitPointSyncParameters>(), *pm);
  PresetManagerUseCases pmUseCases(*pm, *settings);

  useCases.disableSyncSetting();

  Bank* b;
  Preset* presetWithOverlap;
  Preset* presetWithoutOverlap;

  auto createEBWithOverlap = [&] {
    useCases.disableSyncSetting();
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    auto scope = TestHelper::createTestScope();
    sI->stepCPFromHwui(scope->getTransaction(), 2, {});
    CHECK(sI->hasOverlap());
  };

  pmUseCases.createBankAndStoreEditBuffer();
  b = pm->getSelectedBank();
  TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);

  CHECK_FALSE(sI->hasOverlap());
  pmUseCases.appendEditBufferToBank(b);
  presetWithoutOverlap = pm->getSelectedPreset();

  createEBWithOverlap();
  pmUseCases.appendEditBufferToBank(b);
  presetWithOverlap = pm->getSelectedPreset();

  auto syncSetting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();

  WHEN("No Overlap Present")
  {
    useCases.enableSyncSetting();
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);

    WHEN("Preset With Overlap Loaded")
    {
      ebUseCases.load(presetWithOverlap);
      THEN("Sync Setting Disabled")
      {
        CHECK(!syncSetting->get());
      }
    }

    WHEN("Preset Without Overlap Loaded")
    {
      ebUseCases.load(presetWithoutOverlap);
      THEN("Sync Setting Enabled")
      {
        CHECK(syncSetting->get());
      }
    }

    WHEN("gets Stored")
    {
      pmUseCases.appendEditBufferToBank(b);
      THEN("Sync Setting stays Enabled")
      {
        CHECK(syncSetting->get());
      }
    }
  }

  WHEN("Overlap Preset")
  {
    useCases.disableSyncSetting();
    createEBWithOverlap();

    WHEN("Preset With Overlap Loaded")
    {
      ebUseCases.load(presetWithOverlap);
      THEN("Sync Setting Disabled")
      {
        CHECK(!syncSetting->get());
      }
    }

    WHEN("Preset Without Overlap Loaded")
    {
      ebUseCases.load(presetWithoutOverlap);
      THEN("Sync Setting Enabled")
      {
        CHECK(syncSetting->get());
      }
    }

    WHEN("gets Stored")
    {
      pmUseCases.appendEditBufferToBank(b);
      THEN("Sync Setting stays Disabled")
      {
        CHECK(!syncSetting->get());
      }
    }
  }
}

TEST_CASE("Note to Display")
{
  SplitPointDimension dim;
  CHECK(dim.stringizeNote(0) == "C1");
  CHECK(dim.stringizeNote(1) == "C#1");

  CHECK(dim.stringizeNote(60) == "C6");
  CHECK(dim.stringizeNote(30) == "F#3");
  CHECK(dim.stringizeNote(29) == "F3");
}