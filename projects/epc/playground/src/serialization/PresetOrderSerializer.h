#pragma once

#include <serialization/Serializer.h>

class Bank;

class PresetOrderSerializer : public Serializer
{
 public:
  PresetOrderSerializer(Bank *bank, bool ignoreUUIDs);

  static Glib::ustring getTagName();
  void finalize(Reader &reader);

 protected:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  Bank *m_bank;
  bool m_ignoreUUIDs;
};
