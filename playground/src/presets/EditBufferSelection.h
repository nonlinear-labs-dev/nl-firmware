#pragma once
#include <proxies/hwui/HWUI.h>
#include "EditBuffer.h"

class EditBufferSelection : public sigc::trackable {
public:
  EditBufferSelection(EditBuffer* eb);
  void onEditBufferChanged(const EditBuffer* eb);

  void setHWUIEditBufferSelection(VoiceGroup vg);
  VoiceGroup getEditBufferSelection() const;

  sigc::connection onHwuiSelectionChanged(const sigc::slot<void>& s) {
    return m_voiceGroupSelectionChanged.connectAndInit(s);
  }

  void toggleHWEditBufferSelection();

private:

  Signal<void> m_voiceGroupSelectionChanged;
  VoiceGroup m_selectedVG;

  EditBuffer* m_editBuffer;
};
