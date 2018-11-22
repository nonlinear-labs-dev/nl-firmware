#pragma once

#include <libundo/undo/Scope.h>
#include <presets/Preset.h>
#include <presets/PresetBank.h>

class MultiplePresetSelection : public UpdateDocumentContributor
{
 public:
  explicit MultiplePresetSelection(UpdateDocumentContributor* parent);
  void addPreset(UNDO::Scope::tTransactionPtr transaction, PresetBank::tPresetPtr presetToCopy);
  std::vector<PresetBank::tPresetPtr> getPresets() const;
  void writeDocument(Writer&, UpdateDocumentContributor::tUpdateID) const;

 protected:
  std::vector<PresetBank::tPresetPtr> m_presets;
};