#pragma once
#include <sigc++/trackable.h>
#include <nltools/Types.h>
#include <libundo/undo/Transaction.h>
#include <tools/Signal.h>
#include <presets/PresetPartSelection.h>

class Settings;
class EditBuffer;

class VoiceGroupAndLoadToPartManager : public sigc::trackable
{
 public:
  explicit VoiceGroupAndLoadToPartManager(EditBuffer& eb);
  void init();
  void toggleLoadToPart();
  void setLoadToPart(bool state);
  bool isInLoadToPart() const;
  void setCurrentVoiceGroup(UNDO::Transaction* t, VoiceGroup v);
  void setCurrentVoiceGroup(VoiceGroup v);
  void setCurrentVoiceGroupAndUpdateParameterSelection(UNDO::Transaction *transaction, VoiceGroup v);

  VoiceGroup getCurrentVoiceGroup() const;

  void toggleCurrentVoiceGroupAndUpdateParameterSelection();
  void toggleCurrentVoiceGroupAndUpdateParameterSelection(UNDO::Transaction *transaction);

  sigc::connection onCurrentVoiceGroupChanged(const sigc::slot<void, VoiceGroup> &cb);
  sigc::connection onLoadToPartModeChanged(const sigc::slot<void, bool> &cb);

  PresetPartSelection *getPresetPartSelection(VoiceGroup vg);
 private:
  void onPresetLoaded();
  void onEditBufferSoundTypeChanged(SoundType type);

  EditBuffer& m_editBuffer;

  Signal<void, VoiceGroup> m_voiceGoupSignal;
  Signal<void, bool> m_loadToPartSignal;

  sigc::connection m_editBufferSoundTypeConnection;
  sigc::connection m_editBufferPresetLoadedConnection;

  bool m_loadToPartActive = false;
  VoiceGroup m_currentVoiceGroup = VoiceGroup::I;
  std::array<PresetPartSelection, 2> m_partLoad { PresetPartSelection { VoiceGroup::I },
                                                  PresetPartSelection { VoiceGroup::II } };
};
