#include "PresetUseCases.h"
#include <presets/Preset.h>
#include <libundo/undo/Scope.h>
#include <nltools/Assert.h>

PresetUseCases::PresetUseCases(Preset* p)
    : m_preset { p }
{
  nltools_assertAlways(m_preset != nullptr);
}

void PresetUseCases::rename(const std::string& newName)
{
  if(m_preset->getName() != newName)
  {
    auto scope = m_preset->getUndoScope().startTransaction("Rename preset");
    m_preset->setName(scope->getTransaction(), newName);
  }
}

void PresetUseCases::setComment(const Glib::ustring& comment)
{
  if(m_preset->getComment() != comment)
  {
    auto scope = m_preset->getUndoScope().startTransaction("Set Preset Comment");
    m_preset->setAttribute(scope->getTransaction(), "Comment", comment);
  }
}