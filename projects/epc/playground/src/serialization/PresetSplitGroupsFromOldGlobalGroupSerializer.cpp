#include <parameter_declarations.h>
#include <presets/PresetParameter.h>
#include <nltools/logging/Log.h>
#include <tools/ExceptionTools.h>
#include "PresetSplitGroupsFromOldGlobalGroupSerializer.h"

PresetSplitGroupsFromOldGlobalGroupSerializer::PresetSplitGroupsFromOldGlobalGroupSerializer(Preset *preset)
    : PresetParameterGroupSerializer { preset->findParameterGroup({ "Split", VoiceGroup::I }), preset->getType() }
    , m_vgII { preset->findParameterGroup({ "Split", VoiceGroup::II }) }
    , m_preset { preset }
{
}

void PresetSplitGroupsFromOldGlobalGroupSerializer::readTagContent(Reader &reader) const
{
  PresetParameterGroupSerializer::readTagContent(reader);
  reader.onReadFinished(
      [preset = m_preset](auto &reader)
      {
        try
        {
          typedef ParameterId ID;
          auto sI = preset->findParameterByID(ID { C15::PID::Split_Split_Point, VoiceGroup::I }, false);
          auto sII = preset->findParameterByID(ID { C15::PID::Split_Split_Point, VoiceGroup::II }, false);
          if(sI && sII)
          {
            nltools::Log::error("Syncing SplitI into SplitII sI->modAmt:", sI->getModulationAmount(),
                                "sII->modAmt:", sII->getModulationAmount());
            sII->copyFrom(reader.getTransaction(), sI);
          }
        }
        catch(...)
        {
          ExceptionTools::errorLogCurrentException();
        }
      });
}
