#pragma once

#include "Serializer.h"

class ParameterDualGroupSet;

class ParameterGroupsSerializer : public Serializer
{
 public:
  ParameterGroupsSerializer(ParameterDualGroupSet *paramGroups);

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  ParameterDualGroupSet *m_paramGroups;
};
