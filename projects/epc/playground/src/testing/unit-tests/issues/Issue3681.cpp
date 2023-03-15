#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include <presets/Preset.h>
#include <http/UndoScope.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "delete preset and undo does not lead to crash")
{
  MockPresetStorage presets;
  auto singlePreset = presets.getSinglePreset();

  BankUseCases bankUseCases(static_cast<Bank*>(singlePreset->getParent()), *app->getSettings());
  bankUseCases.deletePreset(singlePreset);

  app->getUndoScope()->undo();
  app->getUndoScope()->redo();
  app->getUndoScope()->undo();
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "delete bank and undo does not lead to crash")
{
  MockPresetStorage presets;
  auto singlePreset = presets.getSinglePreset();

  PresetManagerUseCases pmUseCases(*app->getPresetManager(), *app->getSettings());
  pmUseCases.deleteBank(static_cast<Bank*>(singlePreset->getParent()));

  app->getUndoScope()->undo();
  app->getUndoScope()->redo();
  app->getUndoScope()->undo();
}