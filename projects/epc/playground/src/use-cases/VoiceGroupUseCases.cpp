#include "VoiceGroupUseCases.h"
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <use-cases/VoiceGroupAndLoadToPartManager.h>

VoiceGroupUseCases::VoiceGroupUseCases(VoiceGroupAndLoadToPartManager *manager, EditBuffer *eb)
    : m_manager { manager }
    , m_editBuffer { eb }
{
}

void VoiceGroupUseCases::setVoiceGroup(VoiceGroup vg)
{
  auto str = toString(vg);
  auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("Select Part " + str);
  m_manager->setCurrentVoiceGroupAndUpdateParameterSelection(scope->getTransaction(), vg);
}

namespace detail
{
  VoiceGroup invert(VoiceGroup vg)
  {
    if(vg == VoiceGroup::I)
      return VoiceGroup::II;
    else
      return VoiceGroup::I;
  }
}

void VoiceGroupUseCases::toggleVoiceGroupSelection()
{
  setVoiceGroup(detail::invert(m_manager->getCurrentVoiceGroup()));
}
