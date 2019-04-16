#include <presets/EditBuffer.h>
#include "RecallEditBufferSerializer.h"
#include "PresetParameterGroupSerializer.h"

RecallEditBufferSerializer::RecallEditBufferSerializer(EditBuffer *edit)
    : Serializer(getTagName())
    , m_editBuffer(edit)
{
}

Glib::ustring RecallEditBufferSerializer::getTagName() {
    return "recall-parameters";
}

void RecallEditBufferSerializer::writeTagContent(Writer &writer) const {
    for(auto &paramGroup : m_editBuffer->m_recallSet.m_parameterGroups)
    {
        PresetParameterGroupSerializer group(paramGroup.second.get());
        group.write(writer, Attribute("id", paramGroup.first));
    }
    writer.writeTextElement("origin", m_editBuffer->m_recallSet.m_origin);
}

void RecallEditBufferSerializer::readTagContent(Reader &reader) const {
    reader.onTag(PresetParameterGroupSerializer::getTagName(), [&](const Attributes &attr) mutable {
        auto id = attr.get("id");
        auto group = std::make_unique<PresetParameterGroup>();
        auto serializer = new PresetParameterGroupSerializer(group.get());
        m_editBuffer->m_recallSet.m_parameterGroups[id] = std::move(group);
        return serializer;
    });
    reader.onTextElement("origin", [this](const ustring &text, const Attributes &attr) {
        m_editBuffer->m_recallSet.m_origin = text;
    });
}