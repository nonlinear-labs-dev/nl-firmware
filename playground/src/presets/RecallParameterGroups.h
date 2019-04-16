#pragma once

#include "PresetParameterGroups.h"

class ParameterGroupSet;
class EditBuffer;

class RecallParameterGroups : public PresetParameterGroups
{
 public:
  RecallParameterGroups(UpdateDocumentContributor *parent, const EditBuffer &editbuffer);

  PresetParameter *findParameterByID(int id);
  const PresetParameter *findParameterByID(int id) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  void copyParamSet(UNDO::Transaction *transaction, const Preset *other);
  void onPresetDeleted(UNDO::Transaction* transaction);

 private:
  Glib::ustring m_origin = "EditBuffer";
  Glib::ustring s_ebString = "EditBuffer";
  Glib::ustring s_presetString = "Preset";
  tUpdateID m_lastChangeID;

  friend class EditBuffer;
  friend class RecallEditBufferSerializer;
};
