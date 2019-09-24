#include "RecallParameterGroups.h"
#include "presets/Preset.h"
#include "presets/EditBuffer.h"
#include "libundo/undo/Transaction.h"
#include "xml/Writer.h"
#include "RecallParameter.h"
#include "presets/PresetParameter.h"

RecallParameterGroups::RecallParameterGroups(EditBuffer *editBuffer)
    : UpdateDocumentContributor(editBuffer)
    , m_editBuffer{ editBuffer }
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto &g : m_editBuffer->getParameterGroups(vg))
    {
      for(auto &parameter : g->getParameters())
      {
        m_parameters[static_cast<int>(vg)][parameter->getID()]
            = std::make_unique<RecallParameter>(this, parameter->getID());
      }
    }
  }
}

std::array<RecallParameterGroups::tParameterMap, 2>& RecallParameterGroups::getParameters()
{
  return m_parameters;
}


const RecallParameter *RecallParameterGroups::findParameterByID(int id) const
{
  return findParameterByID(id, m_editBuffer->getVoiceGroupSelection());
}

const RecallParameter *RecallParameterGroups::findParameterByID(int id, VoiceGroup vg) const
{
  return m_parameters[static_cast<int>(vg)].at(id).get();
}

void RecallParameterGroups::copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto &g : other->getParameterGroups(vg))
    {
      for(auto &parameter : g->getParameters())
      {
        m_parameters[static_cast<int>(vg)].at(parameter->getID())->copyFrom(transaction, parameter);
      }
    }
  }
}

void RecallParameterGroups::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  auto changed = getUpdateIDOfLastChange() > knownRevision;
  if(changed)
  {
    writer.writeTag("recall-data", Attribute{ "changed", changed }, [this, &writer, knownRevision] {
      for(auto &parameterpair : m_parameters[static_cast<int>(m_editBuffer->getVoiceGroupSelection())])
      {
        auto &parameter = parameterpair.second;
        parameter->writeDocument(writer, knownRevision);
      }
    });
  }
}

UpdateDocumentContributor::tUpdateID RecallParameterGroups::onChange(uint64_t flags)
{
  m_signalRecallValues.send();
  return UpdateDocumentContributor::onChange(flags);
}
