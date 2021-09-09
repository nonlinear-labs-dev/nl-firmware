#include "FuxieSwarmsTestBank.h"

FuxieSwarmsTestBank::FuxieSwarmsTestBank()
{
  const auto bankFile = getSourceDir() + "/projects/epc/playground/test-resources/Fuxi_Swarms.xml";
  FileInStream stream(bankFile, false);
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCase(*TestHelper::getPresetManager(), *settings);
  m_bank = useCase.importBankFromStream(stream, 0, 0, bankFile, [](auto) {});
}

FuxieSwarmsTestBank::~FuxieSwarmsTestBank()
{
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCase(*TestHelper::getPresetManager(), *settings);
  useCase.deleteBank(m_bank);
}

Preset* FuxieSwarmsTestBank::getPreset(int index)
{
  return m_bank->getPresetAt(index);
}
