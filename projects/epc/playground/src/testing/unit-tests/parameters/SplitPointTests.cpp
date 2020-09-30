#include <parameters/scale-converters/dimension/SplitPointDimension.h>
#include "testing/parameter/TestGroupSet.h"
#include "testing/parameter/TestGroup.h"
#include "testing/TestRootDocument.h"
#include "parameters/SplitPointParameter.h"
#include "testing/TestHelper.h"
#include <catch.hpp>

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

  splitI->setCPFromHwui(transaction, 0);
  splitII->setCPFromHwui(transaction, 0);
  REQUIRE(splitI->getDisplayString() == "C1");
  REQUIRE(splitII->getDisplayString() == "C1");

  splitI->setCPFromHwui(transaction, 0.5);
  splitII->setCPFromHwui(transaction, 0.5);
  REQUIRE(splitI->getDisplayString() == "F#3");
  REQUIRE(splitII->getDisplayString() == "F#3");

  splitI->setCPFromHwui(transaction, 1);
  splitII->setCPFromHwui(transaction, 1);
  REQUIRE(splitI->getDisplayString() == "B5");
  REQUIRE(splitII->getDisplayString() == "B5");
}

TEST_CASE("Note to Display")
{
  SplitPointDimension dim;
  REQUIRE(dim.stringizeNote(0) == "C1");
  REQUIRE(dim.stringizeNote(1) == "C#1");

  REQUIRE(dim.stringizeNote(60) == "C6");
  REQUIRE(dim.stringizeNote(30) == "F#3");
  REQUIRE(dim.stringizeNote(29) == "F3");
}