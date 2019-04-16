#pragma once

#include <serialization/Serializer.h>

class Preset;

class PresetParameterGroupsSerializer : public Serializer
{
 public:
  PresetParameterGroupsSerializer(Preset *preset);

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  Preset *m_preset;
};
