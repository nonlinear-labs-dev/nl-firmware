#pragma once
#include <presets/EditBuffer.h>
#include "ParameterGroupSerializer.h"

class SplitGroupsFromOldGlobalGroupSerializer : public ParameterGroupSerializer
{
 public:
  explicit SplitGroupsFromOldGlobalGroupSerializer(EditBuffer* eb);

 protected:
  void readTagContent(Reader& reader) const override;

 private:
  EditBuffer* m_editBuffer;
};
