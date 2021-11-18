#pragma once

namespace UNDO {
  class Transaction;
};

class Preset;
class PresetManager;
class Settings;

class StoreUseCaseHelper
{
 public:
    static void onStore(UNDO::Transaction* transaction, Preset& preset, PresetManager& pm, Settings& settings);
    static void updateSyncSettingOnPresetStore(UNDO::Transaction* transaction, PresetManager& pm, Settings& settings);
};
