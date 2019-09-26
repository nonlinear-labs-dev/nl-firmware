#include "EditBufferSelection.h"

EditBufferSelection::EditBufferSelection(EditBuffer* eb, HWUI* hwui) : m_editBuffer{eb}, m_hwui{hwui} {
  m_editBuffer->onChange(sigc::bind(sigc::mem_fun(this, &EditBufferSelection::onEditBufferChanged), m_editBuffer));
  m_selectedVG = VoiceGroup::I;
}

void EditBufferSelection::onEditBufferChanged(const EditBuffer* eb) {
  if(eb->getType() == EditBufferType::Single) {
    setHWUIEditBufferSelection(VoiceGroup::I);
  }
}

void EditBufferSelection::setHWUIEditBufferSelection(VoiceGroup vg) {
  if(std::exchange(m_selectedVG, vg) != m_selectedVG) {
    m_voiceGroupSelectionChanged.send(m_selectedVG);
  }
}


VoiceGroup EditBufferSelection::getEditBufferSelection() const {
  return m_selectedVG;
}
