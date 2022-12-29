#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include "testing/parameter/TestGroupSet.h"
#include "testing/parameter/TestGroup.h"
#include "parameters/SplitPointParameter.h"
#include "testing/TestHelper.h"
#include "device-settings/Settings.h"
#include "device-settings/SplitPointSyncParameters.h"
#include <catch.hpp>
#include <parameter_declarations.h>
#include <device-settings/SyncSplitSettingUseCases.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Split Point Display Value")
{
  auto eb = TestHelper::getEditBuffer();

  auto splitI
      = dynamic_cast<SplitPointParameter*>(eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I }));
  auto splitII
      = dynamic_cast<SplitPointParameter*>(eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II }));
  g_assert(splitI != nullptr);
  g_assert(splitII != nullptr);

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

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Split Point Helper Functions")
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

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Sync Setting gets updated on store and load")
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
  BankUseCases bankUseCases(b, *TestHelper::getSettings());
  TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);

  CHECK_FALSE(sI->hasOverlap());
  presetWithoutOverlap = bankUseCases.appendEditBuffer();
  CHECK(presetWithoutOverlap == pm->getSelectedPreset());

  createEBWithOverlap();
  presetWithOverlap = bankUseCases.appendEditBuffer();
  CHECK(presetWithOverlap == pm->getSelectedPreset());

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
      bankUseCases.appendEditBuffer();
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
      bankUseCases.appendEditBuffer();
      THEN("Sync Setting stays Disabled")
      {
        CHECK(!syncSetting->get());
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Note to Display")
{
  SplitPointDimension dim;
  CHECK(dim.stringizeNote(0) == "C1");
  CHECK(dim.stringizeNote(1) == "C#1");

  CHECK(dim.stringizeNote(60) == "C6");
  CHECK(dim.stringizeNote(30) == "F#3");
  CHECK(dim.stringizeNote(29) == "F3");
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "modulate split")
{
  auto eb = TestHelper::getEditBuffer();

  auto sI = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto sII = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::II });

  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

  ModParameterUseCases sIUseCases(sI);

  TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);

  sIUseCases.selectModSource(MacroControls::MC1);

  auto modAmount = GENERATE(15.0 / 60.0, 1.0 / 60.0, 59.0 / 60.0);
  sIUseCases.setModulationAmount(modAmount);

  CHECK(sI->getDisplayString() == "F#3");
  CHECK(sII->getDisplayString() == "G3");
  CHECK(sI->getDisplayValue() + 1 == Approx(sII->getDisplayValue()));

  MacroControlParameterUseCases mcUseCases(mcA);
  auto oldRange = sI->getModulationRange(false);

  WHEN("Modulated direct")
  {
    auto mcPos = GENERATE(1.0 / 159.0, 4.0 / 160.0);
    mcUseCases.setControlPosition(mcPos);

    INFO("mcPos: " << mcPos << " modAmount: " << modAmount);

    auto e = std::numeric_limits<float>::epsilon();
    CHECK(sI->getDisplayValue() + 1 == Approx(sII->getDisplayValue()));
    CHECK(oldRange.first == Approx(sI->getModulationRange(false).first).epsilon(e));
    CHECK(oldRange.second == Approx(sI->getModulationRange(false).second).epsilon(e));
  }

  WHEN("Modulated steps fine")
  {
    mcUseCases.setControlPosition(0);

    for(int i = 0; i < 90; i++)
    {
      mcUseCases.incDecPosition(1, true, false);
    }

    CHECK(sI->getDisplayValue() + 1 == Approx(sII->getDisplayValue()));
    CHECK(oldRange.first == Approx(sI->getModulationRange(false).first));
    CHECK(oldRange.second == Approx(sI->getModulationRange(false).second));
  }

  WHEN("Modulated steps coarse")
  {
    mcUseCases.setControlPosition(0);

    for(int i = 0; i < 90; i++)
    {
      mcUseCases.incDecPosition(1, false, false);
    }

    CHECK(sI->getDisplayValue() + 1 == Approx(sII->getDisplayValue()));
    CHECK(oldRange.first == Approx(sI->getModulationRange(false).first));
    CHECK(oldRange.second == Approx(sI->getModulationRange(false).second));
  }
}
