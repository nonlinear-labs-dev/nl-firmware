#pragma once

#include "presets/PresetDualParameterGroups.h"

class ParameterDualGroupSet;
class EditBuffer;
class RecallParameterGroup;
class RecallParameter;

class RecallParameterGroups : public UpdateDocumentContributor
{
 public:
  using tParameterMap = std::unordered_map<int, std::unique_ptr<RecallParameter>>;
  RecallParameterGroups(EditBuffer *editBuffer);

  RecallParameter *findParameterByID(ParameterId id) const;
  tParameterMap &getParameters(VoiceGroup vg);
  const tParameterMap &getParameters(VoiceGroup vg) const;

  void copyFromEditBuffer(UNDO::Transaction *transaction, const EditBuffer *other);
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  tUpdateID onChange(uint64_t flags) override;

 private:
  EditBuffer *m_editBuffer;
  Signal<void> m_signalRecallValues;

  std::array<tParameterMap, static_cast<size_t>(VoiceGroup::Global)> m_parameters;

  friend class EditBuffer;
  friend class RecallEditBufferSerializer;
};
