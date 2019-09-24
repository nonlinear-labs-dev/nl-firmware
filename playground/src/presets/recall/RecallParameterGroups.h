#pragma once

#include "presets/PresetParameterGroups.h"

class ParameterDualGroupSet;
class EditBuffer;
class RecallParameterGroup;
class RecallParameter;

class RecallParameterGroups : public UpdateDocumentContributor
{
 public:
  RecallParameterGroups(EditBuffer *editBuffer);

  const RecallParameter *findParameterByID(int id) const;
  void copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other);
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  tUpdateID onChange(uint64_t flags) override;

 private:
  Signal<void> m_signalRecallValues;

  std::unordered_map<int, std::unique_ptr<RecallParameter>> m_parameters;

  friend class EditBuffer;
  friend class RecallEditBufferSerializer;
};
