#include "RecallParameterGroups.h"
#include "presets/Preset.h"
#include "presets/EditBuffer.h"
#include "libundo/undo/Transaction.h"
#include "xml/Writer.h"
#include "RecallParameter.h"
#include "presets/PresetParameter.h"

RecallParameterGroups::RecallParameterGroups(EditBuffer *editBuffer)
    : UpdateDocumentContributor(editBuffer)
{
  for(auto &g : editBuffer->getParameterGroups())
  {
    for(auto &parameter : g->getParameters())
    {
      m_parameters[parameter->getID()] = std::make_unique<RecallParameter>(this, parameter->getID());
    }
  }
}

const RecallParameter *RecallParameterGroups::findParameterByID(int id) const
{
  return m_parameters.at(id).get();
}

void RecallParameterGroups::copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other)
{
  for(auto &g : other->getParameterGroups())
  {
    for(auto &parameter : g->getParameters())
    {
      m_parameters.at(parameter->getID())->copyFrom(transaction, parameter);
    }
  }
}

void RecallParameterGroups::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  auto changed = getUpdateIDOfLastChange() > knownRevision;
  if(changed)
  {
    writer.writeTag("recall-data", Attribute{ "changed", changed }, [this, &writer, knownRevision] {
      for(auto &parameterpair : m_parameters)
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
