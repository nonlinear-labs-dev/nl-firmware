#include <testing/TestHelper.h>
#include <presets/Bank.h>
#include <presets/Preset.h>

TEST_CASE("Issue 2892")
{
  auto pm = TestHelper::getPresetManager();
  EditBufferUseCases ebUseCase(*pm->getEditBuffer());
  ebUseCase.initSound(Defaults::UserDefault);
  PresetManagerUseCases pmUseCase(*pm, pm->getEditBuffer()->getSettings());
  auto bank = pmUseCase.createBankAndStoreEditBuffer();
  REQUIRE(bank);
  auto firstPreset = bank->getSelectedPreset();
  REQUIRE(firstPreset);

  auto baseName = firstPreset->getName();

  BankUseCases useCase(bank, pm->getEditBuffer()->getSettings());
  for(int i = 0; i < 5; i++)
  {
    auto preset = useCase.appendEditBuffer();
    CHECK(preset->getName() == baseName);
  }

  for(int i = 0; i < 5; i++)
  {
    auto preset = useCase.insertEditBufferAtPosition(0);
    CHECK(preset->getName() == baseName);
  }
}
