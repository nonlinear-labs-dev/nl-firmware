#pragma once

#include <serialization/Serializer.h>
#include <nltools/Types.h>
#include <presets/PresetParameter.h>

class PresetParameterGroup;
class PresetParameterSerializer;

class PresetParameterGroupSerializer : public Serializer
{
 public:
  PresetParameterGroupSerializer(std::vector<PresetParameterGroup *> paramGroup, SoundType type);
  ~PresetParameterGroupSerializer() override;

  static Glib::ustring getTagName();

 protected:
  void writeTagContent(Writer &writer) const override;
  void readTagContent(Reader &reader) const override;

  const std::vector<PresetParameterGroup *> m_paramGroups;
  SoundType m_type;
};
