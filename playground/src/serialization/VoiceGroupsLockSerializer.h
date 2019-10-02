#pragma once

#include <presets/EditBuffer.h>
#include "Serializer.h"

class VoiceGroupsLockSerializer : public Serializer {
public:
  explicit VoiceGroupsLockSerializer(EditBuffer* eb);
  static std::string getTagName();
protected:
  void writeTagContent(Writer &writer) const override;
  void readTagContent(Reader &reader) const override;

private:
  EditBuffer* m_editBuffer;
};
