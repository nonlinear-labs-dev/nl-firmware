#include <http/UpdateDocumentContributor.h>
#include <Application.h>
#include "MultiplePresetSelection.h"

MultiplePresetSelection::MultiplePresetSelection(UpdateDocumentContributor *parent) : UpdateDocumentContributor(parent)
{
}

void MultiplePresetSelection::addPreset(UNDO::Scope::tTransactionPtr transaction, PresetBank::tPresetPtr presetToCopy)
{
  if(presetToCopy) {
    PresetBank::tPresetPtr newPreset(presetToCopy);
    auto swapData = UNDO::createSwapData(newPreset);
    transaction->addSimpleCommand([ = ] (UNDO::Command::State) mutable
    {
      m_presets.push_back (swapData->get<0>());
      onChange();
    }, [ = ] (UNDO::Command::State) mutable
    {
      m_presets.erase (m_presets.end());
      onChange();
    });
  }
}

std::vector<PresetBank::tPresetPtr> MultiplePresetSelection::getPresets() const
{
  return m_presets;
}

void MultiplePresetSelection::writeDocument(Writer&, UpdateDocumentContributor::tUpdateID) const
{
  //Empty on Purpose
}

