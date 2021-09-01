#include "MockPresetStorage.h"
#include <testing/TestHelper.h>
#include <presets/Preset.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <use-cases/PresetUseCases.h>

inline EditBuffer *getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

MockPresetStorage::MockPresetStorage()
{
  m_bank = createBank();
  m_single = createSinglePreset();
  m_layer = createLayerPreset();
  m_split = createSplitPreset();
}

MockPresetStorage::~MockPresetStorage()
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto transaction = scope->getTransaction();
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();
  pm->deleteBank(transaction, m_bank->getUuid());
}

Preset *MockPresetStorage::getSinglePreset()
{
  return m_single;
}

Preset *MockPresetStorage::getSplitPreset()
{
  return m_split;
}

Preset *MockPresetStorage::getLayerPreset()
{
  return m_layer;
}

Preset *MockPresetStorage::createSinglePreset()
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases ebU(*editBuffer);
  ebU.convertToSingle(VoiceGroup::I);

  BankUseCases bankU(m_bank);
  auto preset = bankU.appendEditBuffer();

  PresetUseCases presetU(preset);
  presetU.rename("Single Preset");

  return preset;
}

Preset *MockPresetStorage::createSplitPreset()
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases ebU(*editBuffer);
  ebU.convertToSingle(VoiceGroup::I);
  ebU.convertToSplit(VoiceGroup::I);

  BankUseCases bankU(m_bank);
  auto preset = bankU.appendEditBuffer();

  PresetUseCases presetU(preset);
  presetU.rename("Split Preset");

  return preset;
}

Preset *MockPresetStorage::createLayerPreset()
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases ebU(*editBuffer);
  ebU.convertToLayer(VoiceGroup::I);

  BankUseCases bankU(m_bank);
  auto preset = bankU.appendEditBuffer();

  PresetUseCases presetU(preset);
  presetU.rename("Layer Preset");

  return preset;
}

Bank *MockPresetStorage::getBank()
{
  return m_bank;
}

Bank *MockPresetStorage::createBank()
{
  auto eb = getEditBuffer();
  auto pm = eb->getParent();
  PresetManagerUseCases useCase(pm);
  return useCase.addBank();
}

DualPresetBank::DualPresetBank()
{
  auto pm = Application::get().getPresetManager();
  PresetManagerUseCases pmUseCase(pm);
  m_bank = pmUseCase.addBank();
  BankUseCases bankUseCases(m_bank);

  auto editBuffer = getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);
  useCase.convertToLayer(VoiceGroup::I);

  for(int i = 0; i < 5; i++)
  {
    auto preset = bankUseCases.appendEditBuffer();
    PresetUseCases pUseCase(preset);
    pUseCase.rename("Layer Preset");
    nltools_assertOnDevPC(preset->getType() == SoundType::Layer);
  }
}

DualPresetBank::~DualPresetBank()
{
  auto pm = Application::get().getPresetManager();
  PresetManagerUseCases useCases(pm);
  useCases.deleteBank(m_bank);
}

Bank *DualPresetBank::getBank()
{
  return m_bank;
}