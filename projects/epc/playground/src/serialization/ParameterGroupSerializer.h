#pragma once

#include "Serializer.h"

class ParameterGroup;
class Parameter;

class ParameterGroupSerializer : public Serializer
{
 public:
  explicit ParameterGroupSerializer(ParameterGroup *paramGroup);
  ~ParameterGroupSerializer() override;

  static Glib::ustring getTagName();

 protected:
  void writeTagContent(Writer &writer) const override;
  void readTagContent(Reader &reader) const override;

  ParameterGroup *m_paramGroup = nullptr;
  std::map<int, Parameter *> m_parameterById;
};
