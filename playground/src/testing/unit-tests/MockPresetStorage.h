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

 private:
  Preset* m_single;
  Preset* m_split;
  Preset* m_layer;

  static Preset* createSinglePreset(UNDO::Transaction* transaction);

  static Preset* createSplitPreset(UNDO::Transaction* transaction);

  static Preset* createLayerPreset(UNDO::Transaction* transaction);

  static void removeBankOfPreset(UNDO::Transaction* transaction, Preset* presetToDelete);
};