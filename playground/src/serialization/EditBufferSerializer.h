#pragma once

#include "Serializer.h"

class EditBuffer;

class EditBufferSerializer : public Serializer
{
 public:
  EditBufferSerializer(EditBuffer* editBuffer);
  virtual ~EditBufferSerializer();

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer& writer) const;
  void readTagContent(Reader& reader) const;

  EditBuffer* m_editBuffer;
};
