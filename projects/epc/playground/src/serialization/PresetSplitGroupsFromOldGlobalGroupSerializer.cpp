#include "PresetSplitGroupsFromOldGlobalGroupSerializer.h"

PresetSplitGroupsFromOldGlobalGroupSerializer::PresetSplitGroupsFromOldGlobalGroupSerializer(Preset *preset)
    : PresetParameterGroupSerializer { preset->findParameterGroup({ "Split", VoiceGroup::I }), preset->getType() }
    , m_vgII { preset->findParameterGroup({ "Split", VoiceGroup::II }) }
    , m_preset { preset }
{
}

void PresetSplitGroupsFromOldGlobalGroupSerializer::readTagContent(Reader &reader) const
{
  reader.onReadFinished(
      [preset = m_preset](auto &reader)
      {
        auto sI = preset->findParameterGroup({ "Split", VoiceGroup::I });
        auto sII = preset->findParameterGroup({ "Split", VoiceGroup::II });
        sII->copyFrom(reader.getTransaction(), sI);
      });
  PresetParameterGroupSerializer::readTagContent(reader);
}
