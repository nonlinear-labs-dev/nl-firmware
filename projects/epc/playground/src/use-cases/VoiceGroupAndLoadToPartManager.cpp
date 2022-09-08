#include "VoiceGroupAndLoadToPartManager.h"
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>

VoiceGroupAndLoadToPartManager::VoiceGroupAndLoadToPartManager(EditBuffer &eb)
    : m_editBuffer { eb }
    , m_voiceGoupSignal {}
    , m_currentVoiceGroup { VoiceGroup::I }
{
}

VoiceGroup VoiceGroupAndLoadToPartManager::getCurrentVoiceGroup() const
{
  return m_currentVoiceGroup;
}

void VoiceGroupAndLoadToPartManager::setLoadToPart(bool state)
{
  if(std::exchange(m_loadToPartActive, state) != state)
    m_loadToPartSignal.send(m_loadToPartActive);
}

void VoiceGroupAndLoadToPartManager::setCurrentVoiceGroup(VoiceGroup v)
{
  auto oldGroup = m_currentVoiceGroup;
  if(v == VoiceGroup::I || v == VoiceGroup::II)
    if(std::exchange(m_currentVoiceGroup, v) != v)
    {
      m_voiceGoupSignal.send(m_currentVoiceGroup);
      m_editBuffer.fakeParameterSelectionSignal(oldGroup, m_currentVoiceGroup);
      m_editBuffer.onChange(UpdateDocumentContributor::ChangeFlags::Generic);
    }
}

void VoiceGroupAndLoadToPartManager::setCurrentVoiceGroupAndUpdateParameterSelection(UNDO::Transaction *transaction,
                                                                                     VoiceGroup v)
{
  setCurrentVoiceGroup(v);
  auto selected = m_editBuffer.getSelected(getCurrentVoiceGroup());
  auto id = selected->getID();

  if(id.getVoiceGroup() != VoiceGroup::Global)
  {
    m_editBuffer.undoableSelectParameter(transaction, { id.getNumber(), m_currentVoiceGroup }, false);
  }
}

void VoiceGroupAndLoadToPartManager::toggleCurrentVoiceGroupAndUpdateParameterSelection()
{
  auto currentVG = getCurrentVoiceGroup();
  auto partName = currentVG == VoiceGroup::I ? "II" : "I";
  auto scope = m_editBuffer.getParent()->getUndoScope().startTransaction("Select Part " + std::to_string(partName));
  toggleCurrentVoiceGroupAndUpdateParameterSelection(scope->getTransaction());
}

void VoiceGroupAndLoadToPartManager::toggleCurrentVoiceGroupAndUpdateParameterSelection(UNDO::Transaction *transaction)
{
  if(m_editBuffer.getType() == SoundType::Single)
    return;

  if(m_currentVoiceGroup == VoiceGroup::I)
    setCurrentVoiceGroupAndUpdateParameterSelection(transaction, VoiceGroup::II);
  else if(m_currentVoiceGroup == VoiceGroup::II)
    setCurrentVoiceGroupAndUpdateParameterSelection(transaction, VoiceGroup::I);
}

void VoiceGroupAndLoadToPartManager::toggleCurrentVoiceGroup()
{
  auto scope = m_editBuffer.getParameterFocusLockGuard();

  if(m_editBuffer.getType() == SoundType::Single)
    return;

  if(m_currentVoiceGroup == VoiceGroup::I)
    setCurrentVoiceGroup(VoiceGroup::II);
  else if(m_currentVoiceGroup == VoiceGroup::II)
    setCurrentVoiceGroup(VoiceGroup::I);
}

sigc::connection VoiceGroupAndLoadToPartManager::onCurrentVoiceGroupChanged(const sigc::slot<void, VoiceGroup> &cb)
{
  return m_voiceGoupSignal.connectAndInit(cb, m_currentVoiceGroup);
}

sigc::connection VoiceGroupAndLoadToPartManager::onLoadToPartModeChanged(const sigc::slot<void, bool> &cb)
{
  return m_loadToPartSignal.connectAndInit(cb, m_loadToPartActive);
}

bool VoiceGroupAndLoadToPartManager::isInLoadToPart() const
{
  return m_loadToPartActive;
}

void VoiceGroupAndLoadToPartManager::toggleLoadToPart()
{
  setLoadToPart(!isInLoadToPart());
}

void VoiceGroupAndLoadToPartManager::init()
{
  m_editBufferSoundTypeConnection = m_editBuffer.onSoundTypeChanged(
      sigc::mem_fun(this, &VoiceGroupAndLoadToPartManager::onEditBufferSoundTypeChanged));
  m_editBufferPresetLoadedConnection
      = m_editBuffer.onPresetLoaded(sigc::mem_fun(this, &VoiceGroupAndLoadToPartManager::onPresetLoaded));
}

void VoiceGroupAndLoadToPartManager::onEditBufferSoundTypeChanged(SoundType type)
{
  if(type == SoundType::Single)
  {
    setLoadToPart(false);
  }
}

void VoiceGroupAndLoadToPartManager::onPresetLoaded()
{
  setLoadToPart(false);
}

PresetPartSelection *VoiceGroupAndLoadToPartManager::getPresetPartSelection(VoiceGroup vg)
{
  return &m_partLoad[static_cast<int>(vg)];
}
