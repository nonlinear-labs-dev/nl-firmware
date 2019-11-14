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
    const auto index = static_cast<size_t>(vg);
    for(auto &g : m_editBuffer->getParameterGroups(vg))
    {
      for(auto &parameter : g->getParameters())
      {
        m_polyParameters[index][parameter->getID()] = std::make_unique<RecallParameter>(this, parameter->getID());
      }
    }
  }

  for(auto g : m_editBuffer->getGlobalParameterGroups())
  {
    for(auto &p : g->getParameters())
      m_globalParameters[p->getID()] = std::make_unique<RecallParameter>(this, p->getID());
  }
}

RecallParameterGroups::tParameterMap &RecallParameterGroups::getParameters(VoiceGroup vg)
{
  return m_polyParameters[static_cast<size_t>(vg)];
}

RecallParameterGroups::tParameterMap &RecallParameterGroups::getGlobalParameters()
{
  return m_globalParameters;
}

RecallParameter *RecallParameterGroups::findGlobalParameterByID(int id)
{
  for(auto &p : m_globalParameters)
    if(p.second->getID() == id)
      return p.second.get();

  return nullptr;
}

RecallParameter *RecallParameterGroups::findParameterByID(int id, VoiceGroup vg) const
{
  try
  {
    return m_polyParameters.at(static_cast<size_t>(vg)).at(id).get();
  }
  catch(...)
  {
  }

  try
  {
    return m_globalParameters.at(id).get();
  }
  catch(...)
  {
  }

  if(m_splitPoint && m_splitPoint->getID() == id)
    return m_splitPoint.get();

  return nullptr;
}

void RecallParameterGroups::copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    const auto index = static_cast<size_t>(vg);
    for(auto &g : other->getParameterGroups(vg))
    {
      for(auto &parameter : g->getParameters())
      {
        m_polyParameters[index].at(parameter->getID())->copyFrom(transaction, parameter);
      }
    }
  }

  for(auto &g : other->getGlobalParameterGroups())
  {
    for(auto &p : g->getParameters())
    {
      m_globalParameters.at(p->getID())->copyFrom(transaction, p);
    }
  }

  if(m_splitPoint == nullptr)
    if(other->getSplitPoint() != nullptr)
      m_splitPoint = std::make_unique<RecallParameter>(this, other->getSplitPoint()->getID());

  if(m_splitPoint && other->getSplitPoint())
    m_splitPoint->copyFrom(transaction, other->getSplitPoint());
}

void RecallParameterGroups::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  auto changed = getUpdateIDOfLastChange() > knownRevision;
  if(changed)
  {
    writer.writeTag("recall-data", Attribute{ "changed", changed }, [this, &writer, knownRevision] {
      writer.writeTag("voice-group-I-parameters", [&] {
        for(auto &parameterpair : m_polyParameters[0])
          parameterpair.second->writeDocument(writer, knownRevision);
      });

      writer.writeTag("voice-group-II-parameters", [&] {
        for(auto &parameterpair : m_polyParameters[1])
          parameterpair.second->writeDocument(writer, knownRevision);
      });

      writer.writeTag("global-parameters", [&] {
        for(auto &parameterpair : m_globalParameters)
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
