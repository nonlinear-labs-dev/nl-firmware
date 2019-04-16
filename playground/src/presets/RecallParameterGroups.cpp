#include "RecallParameterGroups.h"
#include "presets/Preset.h"
#include "presets/EditBuffer.h"
#include "libundo/undo/Transaction.h"
#include "xml/Writer.h"

RecallParameterGroups::RecallParameterGroups(UpdateDocumentContributor *parent, const EditBuffer &editbuffer)
    : PresetParameterGroups(parent, editbuffer)
{
}

PresetParameter *RecallParameterGroups::findParameterByID(int id)
{
  return const_cast<PresetParameter *>(const_cast<const RecallParameterGroups *>(this)->findParameterByID(id));
}

const PresetParameter *RecallParameterGroups::findParameterByID(int id) const
{
  for(auto &pair : m_parameterGroups)
  {
    if(auto param = pair.second->findParameterByID(id))
      return param;
  }
  return nullptr;
}

void RecallParameterGroups::copyParamSet(UNDO::Transaction *transaction, const Preset *other)
{
  for(auto &pair : other->m_parameterGroups)
  {
    auto &othergroup = pair.second;
    m_parameterGroups.at(pair.first)->copyFrom(transaction, othergroup.get());
  }

  transaction->addUndoSwap(m_origin, s_presetString);
}

void RecallParameterGroups::onPresetDeleted(UNDO::Transaction *transaction)
{
  transaction->addUndoSwap(m_origin, s_ebString);
}

void RecallParameterGroups::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTag("recall-data", Attribute{"changed", "1"}, [this, &writer] {
    for(auto &pair : m_parameterGroups)
      pair.second->writeDocument(writer);
  });

  AttributesOwner::writeDocument(writer, knownRevision);
}
