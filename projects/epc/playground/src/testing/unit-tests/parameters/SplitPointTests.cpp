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

TEST_CASE("Split Point Display Value")
{
  TestRootDocument root;
  TestGroupSet set { &root };
  TestGroup group(&set, VoiceGroup::I);
  group.addParameter(new SplitPointParameter(&group, ParameterId { 1, VoiceGroup::I }));
  group.addParameter(new SplitPointParameter(&group, ParameterId { 1, VoiceGroup::II }));

  auto splitI = dynamic_cast<SplitPointParameter *>(group.findParameterByID({ 1, VoiceGroup::I }));
  auto splitII = dynamic_cast<SplitPointParameter *>(group.findParameterByID({ 1, VoiceGroup::II }));
  g_assert(splitI != nullptr);

  auto transScope = UNDO::Scope::startTrashTransaction();
  auto transaction = transScope->getTransaction();

  auto syncSetting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  syncSetting->setState(false, transaction);

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
  auto sI = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto sII = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::II });

  WHEN("Init")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>();
    THEN("Default Split Behaviour I|II")
    {
      CHECK(sI->inDefaultSplitBehaviour());
      CHECK(sII->inDefaultSplitBehaviour());
    }

    WHEN("Sync Off and Overlap set")
    {
      auto scope = TestHelper::createTestScope();
      auto setting = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
      setting->setState(false, scope->getTransaction());
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

TEST_CASE("Note to Display")
{
  SplitPointDimension dim;
  CHECK(dim.stringizeNote(0) == "C1");
  CHECK(dim.stringizeNote(1) == "C#1");

  CHECK(dim.stringizeNote(60) == "C6");
  CHECK(dim.stringizeNote(30) == "F#3");
  CHECK(dim.stringizeNote(29) == "F3");
}