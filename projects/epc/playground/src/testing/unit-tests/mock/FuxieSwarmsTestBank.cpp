#include "FuxieSwarmsTestBank.h"

FuxieSwarmsTestBank::FuxieSwarmsTestBank()
{
  const auto bankFile = getSourceDir() + "/projects/epc/playground/test-resources/Fuxi_Swarms.xml";
  FileInStream stream(bankFile, false);
  PresetManagerUseCases useCase(TestHelper::getPresetManager());
  m_bank = useCase.importBankFromStream(stream, 0, 0, bankFile, [](auto) {});
}

FuxieSwarmsTestBank::~FuxieSwarmsTestBank()
{
  PresetManagerUseCases useCase(TestHelper::getPresetManager());
  useCase.deleteBank(m_bank);
}

Preset* FuxieSwarmsTestBank::getPreset(int index)
{
  return m_bank->getPresetAt(index);
}
