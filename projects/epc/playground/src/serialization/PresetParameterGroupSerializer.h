#pragma once

#include <serialization/Serializer.h>
#include <nltools/Types.h>

class PresetParameterGroup;

class PresetParameterGroupSerializer : public Serializer
{
 public:
  PresetParameterGroupSerializer(std::vector<PresetParameterGroup *> paramGroup, SoundType type);
  ~PresetParameterGroupSerializer() override;

  static Glib::ustring getTagName();

 protected:
  void writeTagContent(Writer &writer) const override;
  void readTagContent(Reader &reader) const override;

  std::vector<PresetParameterGroup*> m_paramGroups;
  SoundType m_type;
};
