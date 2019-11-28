#if 0
#pragma once
#include <presets/ParameterDualGroupSet.h>
#include "Serializer.h"

class VoiceGroupsSerializer : public Serializer
{
 public:
  explicit VoiceGroupsSerializer(EditBuffer *editBuffer);

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  EditBuffer *m_editBuffer;
};
#endif