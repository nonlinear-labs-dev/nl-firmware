#include <http/UpdateDocumentContributor.h>
#include <libundo/undo/Transaction.h>
#include <Application.h>
#include "MultiplePresetSelection.h"
#include <presets/Preset.h>

MultiplePresetSelection::MultiplePresetSelection(UpdateDocumentContributor *parent)
    : UpdateDocumentContributor(parent)
{
}

MultiplePresetSelection::~MultiplePresetSelection() = default;

void MultiplePresetSelection::addPreset(UNDO::Transaction *transaction, const Preset *presetToCopy)
{
  if(presetToCopy)
  {
    auto swapData = UNDO::createSwapData(std::make_unique<Preset>(this, *presetToCopy));
    transaction->addSimpleCommand(
        [=](UNDO::Command::State) mutable {
          std::unique_ptr<Preset> p;
          swapData->swapWith(p);
          m_presets.push_back(std::move(p));
          onChange();
        },
        [=](UNDO::Command::State) mutable {
          m_presets.pop_back();
          onChange();
        });
  }
}

const std::list<std::unique_ptr<Preset> > &MultiplePresetSelection::getPresets() const
{
  return m_presets;
}

void MultiplePresetSelection::writeDocument(Writer &, UpdateDocumentContributor::tUpdateID) const
{
  //Empty on Purpose
}
