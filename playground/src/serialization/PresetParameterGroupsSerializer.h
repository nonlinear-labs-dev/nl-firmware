#pragma once

#include <serialization/Serializer.h>

class PresetParameterGroups;

class PresetParameterGroupsSerializer : public Serializer
{
 public:
  PresetParameterGroupsSerializer(PresetParameterGroups *preset);

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  PresetParameterGroups *m_parameterGroups;
};
