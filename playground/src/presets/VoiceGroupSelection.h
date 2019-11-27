#pragma once
#include <proxies/hwui/HWUI.h>

class PresetManager;

class VoiceGroupSelection : public sigc::trackable
{
 public:
  void connectToPresetManager(const PresetManager* p);
  void setHWUIEditBufferSelection(VoiceGroup vg);
  VoiceGroup getEditBufferSelection() const;

  sigc::connection onHwuiSelectionChanged(const sigc::slot<void>& s)
  {
    return m_voiceGroupSelectionChanged.connectAndInit(s);
  }

  void toggleHWEditBufferSelection();

 private:
  void resetSelection();

  Signal<void> m_voiceGroupSelectionChanged;
  VoiceGroup m_selectedVG = VoiceGroup::I;
};
