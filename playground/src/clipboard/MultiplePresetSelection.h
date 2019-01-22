#pragma once

#include <libundo/undo/Scope.h>
#include <presets/Preset.h>
#include <presets/Bank.h>

class MultiplePresetSelection : public UpdateDocumentContributor
{
 public:
  explicit MultiplePresetSelection(UpdateDocumentContributor* parent);
  ~MultiplePresetSelection();

  void addPreset(UNDO::Transaction* transaction, const Preset* presetToCopy);
  const std::list<std::unique_ptr<Preset>>& getPresets() const;
  void writeDocument(Writer&, UpdateDocumentContributor::tUpdateID) const;

 protected:
  std::list<std::unique_ptr<Preset>> m_presets;
};
