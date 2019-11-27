#include "../xml/Attributes.h"
#include "EditBufferSerializer.h"
#include <serialization/ParameterGroupsSerializer.h>
#include "LastLoadedPresetInfoSerializer.h"
#include "presets/EditBuffer.h"
#include "xml/Reader.h"
#include "xml/Writer.h"
#include "PresetParameterGroupsSerializer.h"
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include "serialization/RecallEditBufferSerializer.h"
#include "VoiceGroupsSerializer.h"
#include "VoiceGroupsLockSerializer.h"

EditBufferSerializer::EditBufferSerializer(EditBuffer *editBuffer)
    : Serializer(getTagName())
    , m_editBuffer(editBuffer)
{
}

EditBufferSerializer::~EditBufferSerializer()
{
}

Glib::ustring EditBufferSerializer::getTagName()
{
  return "edit-buffer";
}

void EditBufferSerializer::writeTagContent(Writer &writer) const
{
  SplashLayout::addStatus("Writing Edit Buffer");

  VoiceGroupsSerializer groups(m_editBuffer);
  groups.write(writer);

  if(auto selectedParam = m_editBuffer->getSelected())
    writer.writeTextElement("selected-parameter", selectedParam->getID().toString());

  LastLoadedPresetInfoSerializer lastLoaded(m_editBuffer);
  lastLoaded.write(writer);

  writer.writeTextElement("hash-on-store", to_string(m_editBuffer->m_hashOnStore));

  VoiceGroupsLockSerializer lockSerializer(m_editBuffer);
  lockSerializer.write(writer);

  writer.writeTextElement("editbuffer-type", toString(m_editBuffer->getType()));

  RecallEditBufferSerializer recall(m_editBuffer);
  recall.write(writer);
}

void EditBufferSerializer::readTagContent(Reader &reader) const
{
  SplashLayout::addStatus("Reading Edit Buffer");

  reader.onTextElement("editbuffer-type", [&](auto text, auto) mutable { m_editBuffer->m_type = to<SoundType>(text); });

  reader.onTag(VoiceGroupsSerializer::getTagName(),
               [&](auto) mutable { return new VoiceGroupsSerializer(m_editBuffer); });

  reader.onTextElement("selected-parameter", [&](auto text, auto) mutable {
    m_editBuffer->undoableSelectParameter(reader.getTransaction(), ParameterId(text));
  });

  reader.onTag(LastLoadedPresetInfoSerializer::getTagName(),
               [&](auto) mutable { return new LastLoadedPresetInfoSerializer(m_editBuffer); });

  reader.onTextElement("hash-on-store", [&](auto text, auto) {
    std::stringstream ss(text);
    ss >> m_editBuffer->m_hashOnStore;
    m_editBuffer->onChange();
  });

  reader.onTag(VoiceGroupsLockSerializer::getTagName(),
               [&](auto) mutable { return new VoiceGroupsLockSerializer(m_editBuffer); });

  reader.onTextElement("locked-parameter", [&](auto text, auto) mutable {
#warning "TODO: locked parameters per voice group?"
    //m_editBuffer->findParameterByID(std::stoi(text))->undoableLock(reader.getTransaction());
  });

  reader.onTag(RecallEditBufferSerializer::getTagName(),
               [&](auto) mutable { return new RecallEditBufferSerializer(m_editBuffer); });
}
