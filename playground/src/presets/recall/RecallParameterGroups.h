#pragma once

#include "presets/PresetParameterGroups.h"

class ParameterDualGroupSet;
class EditBuffer;
class RecallParameterGroup;
class RecallParameter;

class RecallParameterGroups : public UpdateDocumentContributor
{
public:
  using tParameterMap = std::unordered_map<int, std::unique_ptr<RecallParameter>>;
  RecallParameterGroups(EditBuffer *editBuffer);

  const RecallParameter *findParameterByID(int id) const;
  const RecallParameter *findParameterByID(int id, VoiceGroup vg) const;
  void copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other);
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  tUpdateID onChange(uint64_t flags) override;

  std::array<tParameterMap, 2>& getParameters();

 private:
  EditBuffer* m_editBuffer;
  Signal<void> m_signalRecallValues;

  std::array<tParameterMap, 2> m_parameters;

  friend class EditBuffer;
  friend class RecallEditBufferSerializer;
};
