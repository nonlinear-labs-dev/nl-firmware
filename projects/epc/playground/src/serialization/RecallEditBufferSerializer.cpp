#include <presets/EditBuffer.h>
#include <nltools/logging/Log.h>
#include "RecallEditBufferSerializer.h"
#include "PresetParameterGroupSerializer.h"
#include <presets/recall/RecallParameter.h>
#include <parameter_declarations.h>

RecallEditBufferSerializer::RecallEditBufferSerializer(EditBuffer *edit)
    : Serializer(getTagName())
    , m_editBuffer(edit)
{
}

Glib::ustring RecallEditBufferSerializer::getTagName()
{
  return "recall-parameters";
}

void RecallEditBufferSerializer::writeTagContent(Writer &writer) const
{
  auto &parameters = m_editBuffer->getRecallParameterSet().getParameters();
  for(auto &param : parameters)
  {
    auto parameter = param.second.get();

    writer.writeTextElement(
        "recall-param-v2", to_string(parameter->getRecallValue()),
        { Attribute("id", parameter->getID().toString()),
          Attribute("mod-amt", to_string(parameter->getRecallModulationAmount())),
          Attribute("mod-src", to_string(parameter->getRecallModSource())),
          Attribute("name", parameter->getGivenName()), Attribute("info", parameter->getInfo()),
          Attribute("ribbon-touch-behaviour", to_string(static_cast<int>(parameter->m_ribbonTouchBehaviour))) });
  }
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const
{
  auto lambda = [=](const Glib::ustring &text, const Attributes &attr) {
    auto id = ParameterId(attr.get("id"));
    auto &rps = m_editBuffer->getRecallParameterSet();

    if(auto param = rps.findParameterByID(id))
    {
      param->m_recallValue = std::stod(text);
      param->m_recallModAmount = std::stod(attr.get("mod-amt"));
      param->m_recallModSource = static_cast<MacroControls>(std::stoi(attr.get("mod-src")));
      param->m_givenName = attr.get("name");
      param->m_info = attr.get("info");
      try
      {
        param->m_ribbonTouchBehaviour
            = static_cast<RibbonTouchBehaviour>(std::stoi(attr.get("ribbon-touch-behaviour")));
      }
      catch(...)
      {
        param->m_ribbonTouchBehaviour = RibbonTouchBehaviour::ABSOLUTE;
      }
    }
    else if(id.getNumber() == C15::PID::Split_Split_Point)
    {
      nltools::Log::warning("Converting old Split group into I and II");
      for(auto vg : { VoiceGroup::I, VoiceGroup::II })
      {
        if(auto split = rps.findParameterByID({ id.getNumber(), vg }))
        {
          split->m_recallValue = std::stod(text);
          split->m_recallModAmount = std::stod(attr.get("mod-amt"));
          split->m_recallModSource = static_cast<MacroControls>(std::stoi(attr.get("mod-amt")));
          split->m_givenName = attr.get("name");
          split->m_info = attr.get("info");
          try
          {
            split->m_ribbonTouchBehaviour
                = static_cast<RibbonTouchBehaviour>(std::stoi(attr.get("ribbon-touch-behaviour")));
          }
          catch(...)
          {
            split->m_ribbonTouchBehaviour = RibbonTouchBehaviour::ABSOLUTE;
          }
        }
      }
    }
  };

  reader.onTextElement("recall-param", lambda);
  reader.onTextElement("recall-param-v2", lambda);
}
