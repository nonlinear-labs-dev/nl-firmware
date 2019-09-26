#pragma once
#include <proxies/hwui/HWUI.h>
#include "EditBuffer.h"

class EditBufferSelection : public sigc::trackable {
public:
  EditBufferSelection(EditBuffer* eb, HWUI* hwui);
  void onEditBufferChanged(const EditBuffer* eb);

  void setHWUIEditBufferSelection(VoiceGroup vg);
  VoiceGroup getEditBufferSelection() const;

  template<class tCB>
  sigc::connection onHwuiSelectionChanged(tCB cb) {
    return m_voiceGroupSelectionChanged.connectAndInit(cb);
  }
private:

  Signal<void, VoiceGroup> m_voiceGroupSelectionChanged;
  VoiceGroup m_selectedVG;

  EditBuffer* m_editBuffer;
  HWUI* m_hwui;
};
