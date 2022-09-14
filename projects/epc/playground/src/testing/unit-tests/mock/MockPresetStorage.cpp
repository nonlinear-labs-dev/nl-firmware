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
  editBuffer->resetOriginIf(editBuffer->getOrigin());
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

  BankUseCases bankU(m_bank, *TestHelper::getSettings());
  auto preset = bankU.appendEditBuffer();

  PresetUseCases presetU(*preset, *TestHelper::getSettings());
  presetU.rename("Single Preset");

  return preset;
}

Preset *MockPresetStorage::createSplitPreset()
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases ebU(*editBuffer);
  ebU.convertToSingle(VoiceGroup::I);
  ebU.convertToSplit(VoiceGroup::I);

  BankUseCases bankU(m_bank, *TestHelper::getSettings());
  auto preset = bankU.appendEditBuffer();

  PresetUseCases presetU(*preset, *TestHelper::getSettings());
  presetU.rename("Split Preset");

  return preset;
}

Preset *MockPresetStorage::createLayerPreset()
{
  auto editBuffer = getEditBuffer();
  EditBufferUseCases ebU(*editBuffer);
  ebU.convertToLayer(VoiceGroup::I);

  BankUseCases bankU(m_bank, *TestHelper::getSettings());
  auto preset = bankU.appendEditBuffer();

  PresetUseCases presetU(*preset, *TestHelper::getSettings());
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
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  return useCase.addBank();
}

DualPresetBank::DualPresetBank()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases pmUseCase(*pm, *settings);
  m_bank = pmUseCase.addBank();
  BankUseCases bankUseCases(m_bank, *TestHelper::getSettings());

  auto editBuffer = getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);
  useCase.convertToLayer(VoiceGroup::I);

  for(int i = 0; i < 5; i++)
  {
    auto preset = bankUseCases.appendEditBuffer();
    PresetUseCases pUseCase(*preset, *TestHelper::getSettings());
    pUseCase.rename("Layer Preset");
    nltools_assertOnDevPC(preset->getType() == SoundType::Layer);
  }
}

DualPresetBank::~DualPresetBank()
{
  auto pm = Application::get().getPresetManager();
  auto settings = TestHelper::getSettings();
  PresetManagerUseCases useCases(*pm, *settings);
  useCases.deleteBank(m_bank);
}

Bank *DualPresetBank::getBank()
{
  return m_bank;
}