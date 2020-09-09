#pragma once

#include <serialization/Serializer.h>

class Bank;

class PresetBankMetadataSerializer : public Serializer
{
 public:
  PresetBankMetadataSerializer(Bank *bank, bool ignoreUUIDs = false);

  static Glib::ustring getTagName();

 protected:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  Bank *m_bank = nullptr;
  bool m_ignoreUUIDs = false;
};
