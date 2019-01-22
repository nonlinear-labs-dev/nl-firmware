#pragma once

#include <serialization/Serializer.h>

class PresetParameterGroup;

class PresetParameterGroupSerializer : public Serializer
{
 public:
  PresetParameterGroupSerializer(PresetParameterGroup *paramGroup);
  virtual ~PresetParameterGroupSerializer();

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  PresetParameterGroup *m_paramGroup = nullptr;
};
