#pragma once
#include <libundo/undo/Transaction.h>

class Preset;

class MockPresetStorage
{
 public:
  MockPresetStorage();

  ~MockPresetStorage();

  Preset* getSinglePreset();

  Preset* getSplitPreset();

  Preset* getLayerPreset();

  static Preset* createSinglePreset(UNDO::Transaction* transaction);
  static Preset* createSplitPreset(UNDO::Transaction* transaction);
  static Preset* createLayerPreset(UNDO::Transaction* transaction);

private:
  Preset* m_single;
  Preset* m_split;
  Preset* m_layer;

  static void removeBankOfPreset(UNDO::Transaction* transaction, Preset* presetToDelete);
};