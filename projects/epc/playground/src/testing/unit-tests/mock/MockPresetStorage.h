#pragma once
#include <libundo/undo/Transaction.h>
#include <tools/Uuid.h>
#include <presets/Bank.h>

class Preset;

class MockPresetStorage
{
 public:
  MockPresetStorage();

  ~MockPresetStorage();

  Preset* getSinglePreset();
  Preset* getSplitPreset();
  Preset* getLayerPreset();

  Preset* createSinglePreset();
  Preset* createSplitPreset();
  Preset* createLayerPreset();

  Bank* getBank();

 private:
  Preset* m_single;
  Preset* m_split;
  Preset* m_layer;
  Bank* m_bank;

  void removeBankOfPreset(UNDO::Transaction* transaction, Preset* presetToDelete);
  Bank* createBank();
};

class DualPresetBank
{
 public:
  DualPresetBank();
  ~DualPresetBank();

  Bank* getBank();

 private:
  Bank* m_bank;
};