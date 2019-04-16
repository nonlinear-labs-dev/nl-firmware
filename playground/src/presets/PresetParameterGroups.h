#pragma once

#include <http/UpdateDocumentContributor.h>
#include "AttributesOwner.h"
#include "PresetParameterGroup.h"

class EditBuffer;
class Preset;

class PresetParameterGroups : public AttributesOwner
{
 public:
  PresetParameterGroups(UpdateDocumentContributor* parent);
  PresetParameterGroups(UpdateDocumentContributor* parent, const Preset& other);
  PresetParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& editbuffer);
  void init(const EditBuffer& editbuffer);
  void init(const Preset& preset);
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  PresetParameter* findParameterByID(int id);
  const PresetParameter* findParameterByID(int id) const;

  void copyParamSet(UNDO::Transaction* transaction, const Preset* other);

 protected:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  std::unordered_map<std::string, GroupPtr> m_parameterGroups;
};