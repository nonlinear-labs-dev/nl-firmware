#pragma once

#include <serialization/Serializer.h>
#include <nltools/Types.h>

class PresetParameter;

class PresetParameterSerializer : public Serializer
{
 public:
  PresetParameterSerializer(std::vector<PresetParameter*> param, SoundType type);

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer& writer) const override;
  void readTagContent(Reader& reader) const override;

  std::vector<PresetParameter*> m_param;
  SoundType m_type;
};
