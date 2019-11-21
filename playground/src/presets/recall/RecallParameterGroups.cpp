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
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    const auto index = static_cast<size_t>(vg);
    for(auto &g : m_editBuffer->getParameterGroups(vg))
    {
      for(auto &parameter : g->getParameters())
      {
        m_parameters[index][parameter->getID()] = std::make_unique<RecallParameter>(this, parameter->getID());
      }
    }
  }
}

RecallParameterGroups::tParameterMap &RecallParameterGroups::getParameters(VoiceGroup vg)
{
  return m_parameters[static_cast<size_t>(vg)];
}

const RecallParameterGroups::tParameterMap &RecallParameterGroups::getParameters(VoiceGroup vg) const
{
  return m_parameters[static_cast<size_t>(vg)];
}

RecallParameter *RecallParameterGroups::findParameterByID(ParameterId id) const
{
  return m_parameters.at(static_cast<size_t>(vg.getVoiceGroup())).at(id.getNumber()).get();
}

void RecallParameterGroups::copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other)
{
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &g : other->getParameterGroups(vg))
      for(auto &parameter : g->getParameters())
        m_parameters[static_cast<size_t>(vg)].at(parameter->getID())->copyFrom(transaction, parameter);
}

void RecallParameterGroups::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  auto changed = getUpdateIDOfLastChange() > knownRevision;
  if(changed)
  {
    writer.writeTag("recall-data", Attribute{ "changed", changed }, [this, &writer, knownRevision] {
      writer.writeTag("voice-group-I-parameters", [&] {
        for(auto &parameterpair : getParameters(VoiceGroup::I))
          parameterpair.second->writeDocument(writer, knownRevision);
      });

      writer.writeTag("voice-group-II-parameters", [&] {
        for(auto &parameterpair : getParameters(VoiceGroup::II))
          parameterpair.second->writeDocument(writer, knownRevision);
      });

      writer.writeTag("global-parameters", [&] {
        for(auto &parameterpair : getParameters(VoiceGroup::Global))
          parameterpair.second->writeDocument(writer, knownRevision);
      });
    });
  }
}

UpdateDocumentContributor::tUpdateID RecallParameterGroups::onChange(uint64_t flags)
{
  m_signalRecallValues.send();
  return UpdateDocumentContributor::onChange(flags);
}
