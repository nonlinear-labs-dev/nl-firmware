#include "VoiceGroupSelection.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"

void VoiceGroupSelection::connectToPresetManager(const PresetManager* p)
{
  p->getEditBuffer()->onPresetLoaded(sigc::mem_fun(this, &VoiceGroupSelection::resetSelection));
}

void VoiceGroupSelection::setHWUIEditBufferSelection(VoiceGroup vg)
{
  if(std::exchange(m_selectedVG, vg) != vg)
  {
    m_voiceGroupSelectionChanged.send();
  }
}

void VoiceGroupSelection::resetSelection()
{
  setHWUIEditBufferSelection(VoiceGroup::I);
}

VoiceGroup VoiceGroupSelection::getEditBufferSelection() const
{
  return m_selectedVG;
}

void VoiceGroupSelection::toggleHWEditBufferSelection()
{
  setHWUIEditBufferSelection(m_selectedVG == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I);
}
