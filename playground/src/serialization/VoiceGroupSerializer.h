#pragma once
#include <presets/ParameterDualGroupSet.h>
#include "Serializer.h"

class VoiceGroupSerializer : public Serializer {
public:
  explicit VoiceGroupSerializer(ParameterDualGroupSet *paramGroups);

  static Glib::ustring getTagName();

private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  ParameterDualGroupSet *m_paramGroups;
};
