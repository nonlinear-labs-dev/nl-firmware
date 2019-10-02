#include "VoiceGroupSelection.h"

void VoiceGroupSelection::setHWUIEditBufferSelection(VoiceGroup vg)
{
  if(std::exchange(m_selectedVG, vg) != vg)
  {
    m_voiceGroupSelectionChanged.send();
  }
}

VoiceGroup VoiceGroupSelection::getEditBufferSelection() const
{
  return m_selectedVG;
}

void VoiceGroupSelection::toggleHWEditBufferSelection()
{
  setHWUIEditBufferSelection(m_selectedVG == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I);
}
