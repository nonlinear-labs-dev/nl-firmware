#pragma once

#include <presets/EditBuffer.h>
#include "Serializer.h"

class VoiceGroupLockSerializer : public Serializer {
public:
  explicit VoiceGroupLockSerializer(EditBuffer* eb);
  static std::string getTagName();
protected:
  void writeTagContent(Writer &writer) const override;
  void readTagContent(Reader &reader) const override;

private:
  EditBuffer* m_editBuffer;
};
