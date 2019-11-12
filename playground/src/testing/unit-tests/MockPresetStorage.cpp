#include "MockPresetStorage.h"
#include <testing/TestHelper.h>
#include <presets/Preset.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>

inline EditBuffer* getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

MockPresetStorage::MockPresetStorage()
{
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  m_layer = createLayerPreset(transaction);
  m_split = createSplitPreset(transaction);
  m_single = createSinglePreset(transaction);
}

MockPresetStorage::~MockPresetStorage()
{
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  removeBankOfPreset(transaction, m_single);
  removeBankOfPreset(transaction, m_split);
  removeBankOfPreset(transaction, m_layer);
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

Preset *MockPresetStorage::createSinglePreset(UNDO::Transaction *transaction)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  editBuffer->undoableConvertToSingle(transaction, VoiceGroup::I);
  auto bank = pm->addBank(transaction);
  auto preset = bank->appendPreset(transaction, std::make_unique<Preset>(bank, *editBuffer));
  preset->setName(transaction, "Single Preset");
  return preset;
}

Preset *MockPresetStorage::createSplitPreset(UNDO::Transaction *transaction)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  editBuffer->undoableConvertToSingle(transaction, VoiceGroup::I);

  editBuffer->undoableConvertToDual(transaction, SoundType::Split, VoiceGroup::I);

  auto bank = pm->addBank(transaction);
  auto preset = bank->appendPreset(transaction, std::make_unique<Preset>(bank, *editBuffer));
  preset->setName(transaction, "Split Preset");
  return preset;
}

Preset *MockPresetStorage::createLayerPreset(UNDO::Transaction *transaction)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();

  editBuffer->undoableConvertToDual(transaction, SoundType::Layer, VoiceGroup::I);
  auto bank = pm->addBank(transaction);
  auto preset = bank->appendPreset(transaction, std::make_unique<Preset>(bank, *editBuffer));
  preset->setName(transaction, "Layer Preset");
  return preset;
}

void MockPresetStorage::removeBankOfPreset(UNDO::Transaction *transaction, Preset *presetToDelete)
{
  auto editBuffer = getEditBuffer();
  auto pm = editBuffer->getParent();
  if(auto bank = dynamic_cast<Bank *>(presetToDelete->getParent()))
    pm->deleteBank(transaction, bank->getUuid());
}
