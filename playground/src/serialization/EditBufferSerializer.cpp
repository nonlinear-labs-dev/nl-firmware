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
#include "VoiceGroupLockSerializer.h"

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

  writer.writeTextElement("editbuffer-type", toString(m_editBuffer->getType()));

  VoiceGroupsSerializer groups(m_editBuffer);
  groups.write(writer);

  if(auto selectedParam = m_editBuffer->getSelected())
    writer.writeTextElement("selected-parameter", to_string(selectedParam->getID()));

  LastLoadedPresetInfoSerializer lastLoaded(m_editBuffer);
  lastLoaded.write(writer);

  writer.writeTextElement("hash-on-store", to_string(m_editBuffer->m_hashOnStore));

  VoiceGroupLockSerializer lockSerializer(m_editBuffer);
  lockSerializer.write(writer);

  writer.writeTextElement("editbuffer-type", toString(m_editBuffer->getType()));

  RecallEditBufferSerializer recall(m_editBuffer);
  recall.write(writer);
}

void EditBufferSerializer::readTagContent(Reader &reader) const
{
  //TODO Assert all parameters unlocked!?

  SplashLayout::addStatus("Reading Edit Buffer");

  reader.onTextElement("editbuffer-type", [&](auto text, auto) mutable {
    m_editBuffer->undoableConvertToType(reader.getTransaction(), toSoundType(text));
  });

  reader.onTextElement("locked-parameter", [&](auto text, auto) mutable {
    m_editBuffer->findParameterByID(std::stoi(text))->undoableLock(reader.getTransaction());
  });

  reader.onTextElement("editbuffer-type",
                       [&](auto text, auto) mutable { m_editBuffer->undoableConvertToType(toSoundType(text)); });

  reader.onTag(VoiceGroupsSerializer::getTagName(),
               [&](auto) mutable { return new VoiceGroupsSerializer(m_editBuffer); });

  reader.onTextElement("selected-parameter", [&](auto text, auto) mutable {
    m_editBuffer->undoableSelectParameter(reader.getTransaction(), text);
  });

  reader.onTag(LastLoadedPresetInfoSerializer::getTagName(),
               [&](auto) mutable { return new LastLoadedPresetInfoSerializer(m_editBuffer); });

  reader.loadElement("hash-on-store", m_editBuffer->m_hashOnStore);

  reader.onTag(VoiceGroupLockSerializer::getTagName(),
               [&](auto) mutable { return new VoiceGroupLockSerializer(m_editBuffer); });

  reader.onTextElement("locked-parameter", [&](auto text, auto) mutable {
    m_editBuffer->findParameterByID(std::stoi(text))->undoableLock(reader.getTransaction());
  });

  reader.onTag(RecallEditBufferSerializer::getTagName(),
               [&](auto) mutable { return new RecallEditBufferSerializer(m_editBuffer); });
}
