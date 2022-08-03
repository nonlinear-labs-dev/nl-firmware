#pragma once
#include <sigc++/trackable.h>
#include <nltools/Types.h>
#include <libundo/undo/Transaction.h>
#include <tools/Signal.h>

class Settings;
class EditBuffer;

class VoiceGroupAndLoadToPartManager : public sigc::trackable
{
 public:
  VoiceGroupAndLoadToPartManager(EditBuffer& eb);
  void init();
  void toggleLoadToPart();
  void setLoadToPart(bool state);
  bool isInLoadToPart() const; //MOVE
  void setCurrentVoiceGroup(VoiceGroup v);
  void setCurrentVoiceGroupAndUpdateParameterSelection(UNDO::Transaction *transaction, VoiceGroup v);

  VoiceGroup getCurrentVoiceGroup() const; //MOVE

  void toggleCurrentVoiceGroupAndUpdateParameterSelection();
  void toggleCurrentVoiceGroupAndUpdateParameterSelection(UNDO::Transaction *transaction);
  void toggleCurrentVoiceGroup();

  sigc::connection onCurrentVoiceGroupChanged(const sigc::slot<void, VoiceGroup> &cb);
  sigc::connection onLoadToPartModeChanged(const sigc::slot<void, bool> &cb);
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
};
