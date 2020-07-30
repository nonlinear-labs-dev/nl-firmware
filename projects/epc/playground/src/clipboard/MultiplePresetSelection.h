#pragma once

#include <http/UpdateDocumentContributor.h>
#include <memory>
#include <list>

class Preset;

namespace UNDO
{
  class Transaction;
}

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
