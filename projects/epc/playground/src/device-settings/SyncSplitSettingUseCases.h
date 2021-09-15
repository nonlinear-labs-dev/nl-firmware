#pragma once
#include <libundo/undo/Transaction.h>

class SplitPointSyncParameters;
class SplitPointParameter;
class PresetManager;

class SyncSplitSettingUseCases
{
 public:
  SyncSplitSettingUseCases(SplitPointSyncParameters& setting, PresetManager& pm);

  void updateFromWebUI(const Glib::ustring& value);
  void enableSyncSetting();
  void disableSyncSetting();
  void enableSyncSetting(UNDO::Transaction*);
  void disableSyncSetting(UNDO::Transaction*);

  void toggleSyncSetting();

 private:
  void undoableSetSetting(UNDO::Transaction* transaction, bool state);

  SplitPointSyncParameters& m_setting;
  PresetManager& m_presetManager;

  SplitPointParameter* m_splitI;
  SplitPointParameter* m_splitII;
};
