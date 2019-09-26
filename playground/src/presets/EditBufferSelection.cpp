#include "EditBufferSelection.h"

EditBufferSelection::EditBufferSelection(EditBuffer* eb)
    : m_editBuffer{ eb }
    , m_selectedVG{ VoiceGroup::I }
{
  m_editBuffer->onChange(sigc::bind(sigc::mem_fun(this, &EditBufferSelection::onEditBufferChanged), m_editBuffer));
}

void EditBufferSelection::onEditBufferChanged(const EditBuffer* eb)
{
  //TODO
}

void EditBufferSelection::setHWUIEditBufferSelection(VoiceGroup vg)
{
  if(std::exchange(m_selectedVG, vg) != vg)
  {
    m_voiceGroupSelectionChanged.send();
    m_editBuffer->onChange(); //Implizit ändert er sich ja auch...
  }
}

VoiceGroup EditBufferSelection::getEditBufferSelection() const
{
  return m_selectedVG;
}

void EditBufferSelection::toggleHWEditBufferSelection()
{
  setHWUIEditBufferSelection(m_selectedVG == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I);
}
