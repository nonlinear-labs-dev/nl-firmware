#include "testing/TestHelper.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <groups/ParameterGroup.h>
#include <libundo/undo/TrashTransaction.h>
#include <parameter_list.h>
#include <use-cases/PresetManagerUseCases.h>
#include <use-cases/BankUseCases.h>
#include <use-cases/PresetUseCases.h>
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>

TEST_CASE("Undo copyFrom")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  auto settings = TestHelper::getSettings();

  PresetManagerUseCases pmUC(*pm, *settings);
  auto bank = pmUC.addBank();

  BankUseCases bankUC(bank, *settings);
  auto preset = bankUC.appendEditBuffer();

  ParameterId paramId = { C15::PID::Env_A_Att, VoiceGroup::I };

  {
    auto scope = pm->getUndoScope().startTransaction("Init State");
    preset->findParameterByID(paramId, false)->setValue(scope->getTransaction(), 1.0);
    eb->findParameterByID(paramId)->setCPFromHwui(scope->getTransaction(), 0.5);
  }

  WHEN("parameter group already exists")
  {
    PresetUseCases uc(*preset, *settings);
    uc.overwriteWithEditBuffer(*eb);
    CHECK(preset->findParameterByID(paramId, false)->getValue() == 0.5);

    THEN("Undo restores old value")
    {
      pm->getUndoScope().undo();
      CHECK(preset->findParameterByID(paramId, false)->getValue() == 1.0);
    }
  }

  WHEN("parameter group is missing")
  {
    PresetUseCases uc(*preset, *settings);
    GroupId groupId = { "Env A", VoiceGroup::I };
    preset->TEST_deleteGroup(groupId);
    CHECK(preset->findParameterGroup(groupId) == nullptr);
    CHECK(preset->findParameterByID(paramId, false) == nullptr);

    THEN("group is added by 'copyFrom'")
    {
      uc.overwriteWithEditBuffer(*eb);

      CHECK(preset->findParameterGroup(groupId) != nullptr);
      CHECK(preset->findParameterByID(paramId, false) != nullptr);
      CHECK(preset->findParameterByID(paramId, false)->getValue() == 0.5);

      THEN("Undo restores 'group missing state'")
      {
        pm->getUndoScope().undo();
        CHECK(preset->findParameterGroup(groupId) == nullptr);
        CHECK(preset->findParameterByID(paramId, false) == nullptr);

        THEN("Redo restores group")
        {
          pm->getUndoScope().redo();
          CHECK(preset->findParameterGroup(groupId) != nullptr);
          CHECK(preset->findParameterByID(paramId, false) != nullptr);
          CHECK(preset->findParameterByID(paramId, false)->getValue() == 0.5);
        }
      }
    }
  }

  WHEN("parameter is missing")
  {
    PresetUseCases uc(*preset, *settings);
    GroupId groupId = { "Env A", VoiceGroup::I };
    preset->findParameterGroup(groupId)->TEST_deleteParameter(paramId);
    CHECK(preset->findParameterByID(paramId, false) == nullptr);
    uc.overwriteWithEditBuffer(*eb);

    THEN("parameter is added by 'copyFrom'")
    {
      CHECK(preset->findParameterGroup(groupId) != nullptr);
      CHECK(preset->findParameterByID(paramId, false) != nullptr);
      CHECK(preset->findParameterByID(paramId, false)->getValue() == 0.5);

      THEN("Undo restores 'param missing state'")
      {
        pm->getUndoScope().undo();
        CHECK(preset->findParameterGroup(groupId) != nullptr);
        CHECK(preset->findParameterByID(paramId, false) == nullptr);

        THEN("Redo restores param")
        {
          pm->getUndoScope().redo();
          CHECK(preset->findParameterGroup(groupId) != nullptr);
          CHECK(preset->findParameterByID(paramId, false) != nullptr);
          CHECK(preset->findParameterByID(paramId, false)->getValue() == 0.5);
        }
      }
    }
  }

  WHEN("parameter group is missing in VGII")
  {
    PresetUseCases uc(*preset, *settings);
    GroupId groupId = { "Env A", VoiceGroup::II };
    ParameterId paramId = { C15::PID::Env_A_Att, VoiceGroup::II };

    preset->TEST_deleteGroup(groupId);
    CHECK(preset->findParameterGroup(groupId) == nullptr);
    CHECK(preset->findParameterByID(paramId, false) == nullptr);

    THEN("group is added by 'copyVoiceGroup1IntoVoiceGroup2'")
    {
      {
        auto scope = pm->getUndoScope().startTransaction("Foo");
        preset->copyVoiceGroup1IntoVoiceGroup2(scope->getTransaction(), {});
      }

      CHECK(preset->findParameterGroup(groupId) != nullptr);
      CHECK(preset->findParameterByID(paramId, false) != nullptr);
      CHECK(preset->findParameterByID(paramId, false)->getValue() == 1.0);

      THEN("Undo restores 'group missing state'")
      {
        pm->getUndoScope().undo();
        CHECK(preset->findParameterGroup(groupId) == nullptr);
        CHECK(preset->findParameterByID(paramId, false) == nullptr);

        THEN("Redo restores group")
        {
          pm->getUndoScope().redo();
          CHECK(preset->findParameterGroup(groupId) != nullptr);
          CHECK(preset->findParameterByID(paramId, false) != nullptr);
          CHECK(preset->findParameterByID(paramId, false)->getValue() == 1.0);
        }
      }
    }
  }
}

TEST_CASE("PresetParameterGroup")
{
  WHEN("VoiceGroup is assigned to PresetParameterGroup")
  {
    struct PGroup : public ParameterGroup
    {
      PGroup()
          : ParameterGroup(TestHelper::getEditBuffer(), { "A", VoiceGroup::I }, "Foo", "Bar", "Baz")
      {
      }

      void init()
      {
        appendParameter(new Parameter(this, { 1, VoiceGroup::I }, ScaleConverter::get<Linear100PercentScaleConverter>()));
      }
    };

    PGroup pGroup;
    pGroup.init();
    PresetParameterGroup group(pGroup);
    CHECK(group.findParameterByID({ 1, VoiceGroup::I }) != nullptr);
    CHECK(group.findParameterByID({ 2, VoiceGroup::I }) == nullptr);
    CHECK(group.findParameterByID({ 1, VoiceGroup::II }) == nullptr);

    auto &undo = TestHelper::getPresetManager()->getUndoScope();

    {
      auto scope = undo.startTransaction("Foo");
      group.assignVoiceGroup(scope->getTransaction(), VoiceGroup::II);
    }

    THEN("Parameter has new VoiceGroup")
    {
      CHECK(group.findParameterByID({ 1, VoiceGroup::I }) == nullptr);
      CHECK(group.findParameterByID({ 1, VoiceGroup::II }) != nullptr);
      CHECK(group.findParameterByID({ 1, VoiceGroup::II })->getID().getVoiceGroup() == VoiceGroup::II);

      WHEN("Undo is performed")
      {
        undo.undo();

        THEN("Parameters voice group is unrolled")
        {
          CHECK(group.findParameterByID({ 1, VoiceGroup::II }) == nullptr);
          CHECK(group.findParameterByID({ 1, VoiceGroup::I }) != nullptr);
          CHECK(group.findParameterByID({ 1, VoiceGroup::I })->getID().getVoiceGroup() == VoiceGroup::I);

          WHEN("Redo is performed")
          {
            undo.redo();

            THEN("Parameters voice group is unrolled")
            {
              CHECK(group.findParameterByID({ 1, VoiceGroup::I }) == nullptr);
              CHECK(group.findParameterByID({ 1, VoiceGroup::II }) != nullptr);
              CHECK(group.findParameterByID({ 1, VoiceGroup::II })->getID().getVoiceGroup() == VoiceGroup::II);
            }
          }
        }
      }
    }
  }
}

