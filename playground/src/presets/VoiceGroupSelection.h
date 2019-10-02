#pragma once
#include <proxies/hwui/HWUI.h>

class VoiceGroupSelection : public sigc::trackable
{
 public:
  void setHWUIEditBufferSelection(VoiceGroup vg);
  VoiceGroup getEditBufferSelection() const;

  sigc::connection onHwuiSelectionChanged(const sigc::slot<void>& s)
  {
    return m_voiceGroupSelectionChanged.connectAndInit(s);
  }

  void toggleHWEditBufferSelection();

 private:
  Signal<void> m_voiceGroupSelectionChanged;
  VoiceGroup m_selectedVG = VoiceGroup::I;
};
