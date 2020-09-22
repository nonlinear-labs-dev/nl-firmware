#include "SplitGroupsFromOldGlobalGroupSerializer.h"

SplitGroupsFromOldGlobalGroupSerializer::SplitGroupsFromOldGlobalGroupSerializer(EditBuffer* eb)
    : ParameterGroupSerializer { eb->getParameterGroupByID({ "Split", VoiceGroup::I }) }
    , m_editBuffer { eb }
{
}

void SplitGroupsFromOldGlobalGroupSerializer::readTagContent(Reader& reader) const
{
  ParameterGroupSerializer::readTagContent(reader);
  auto other = m_editBuffer->getParameterGroupByID({ "Split", VoiceGroup::II });
  other->copyFrom(reader.getTransaction(), m_paramGroup);
}
