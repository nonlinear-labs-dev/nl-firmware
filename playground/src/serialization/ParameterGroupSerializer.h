#pragma once

#include "Serializer.h"

class ParameterGroup;
class Parameter;

class ParameterGroupSerializer : public Serializer
{
 public:
  ParameterGroupSerializer(ParameterGroup *paramGroup);
  virtual ~ParameterGroupSerializer();

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  ParameterGroup *m_paramGroup = nullptr;
  std::map<int, Parameter *> m_parameterById;
};
