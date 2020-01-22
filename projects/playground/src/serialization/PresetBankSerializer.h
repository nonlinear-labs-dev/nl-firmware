#pragma once

#include "PresetBankMetadataSerializer.h"

class Bank;

class PresetBankSerializer : public PresetBankMetadataSerializer
{
 private:
  typedef PresetBankMetadataSerializer super;

 public:
  PresetBankSerializer(Bank *bank, bool ignoreUUIDs = false);

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;
};
