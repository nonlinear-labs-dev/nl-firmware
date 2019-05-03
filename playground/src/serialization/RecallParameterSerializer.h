#pragma once

#include "Serializer.h"

class RecallParameter;

class RecallParameterSerializer : public Serializer
{
 public:
  RecallParameterSerializer(RecallParameter* target);

  static Glib::ustring getTagName();

 protected:
  void writeTagContent(Writer& writer) const override;
  void readTagContent(Reader& reader) const override;

 private:
  RecallParameter* m_parameter;
};
