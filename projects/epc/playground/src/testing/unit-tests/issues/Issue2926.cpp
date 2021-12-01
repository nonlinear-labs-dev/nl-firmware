#include <testing/TestHelper.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include "use-cases/PresetUseCases.h"

TEST_CASE("Issue 2926")
{
  auto pm = TestHelper::getPresetManager();
  EditBufferUseCases ebUseCase(*pm->getEditBuffer());
  ebUseCase.initSound(Defaults::UserDefault);
  auto& settings = pm->getEditBuffer()->getSettings();
  PresetManagerUseCases pmUseCase(*pm, settings);

  auto bank = pmUseCase.createBankAndStoreEditBuffer();
  REQUIRE(bank);
  BankUseCases bankUseCase(bank, settings);
  auto firstPreset = bankUseCase.appendEditBuffer();
  bankUseCase.appendEditBuffer();
  bankUseCase.appendEditBuffer();
  auto lastPreset = bankUseCase.appendEditBuffer();

  pmUseCase.selectPreset(firstPreset);
  auto baseName = firstPreset->getName();


  CHECK(bank->getSelectedPreset() == firstPreset);

  PresetUseCases presetUseCase(lastPreset, settings);
  presetUseCase.overwriteWithEditBuffer(*pm->getEditBuffer());

  CHECK(bank->getSelectedPreset() == lastPreset);
}
