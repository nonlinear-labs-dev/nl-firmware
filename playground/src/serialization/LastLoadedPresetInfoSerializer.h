#pragma once

#include "Serializer.h"

class EditBuffer;

class LastLoadedPresetInfoSerializer : public Serializer
{
 public:
  LastLoadedPresetInfoSerializer(EditBuffer *eb);
  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  EditBuffer *m_editBuffer;
};
