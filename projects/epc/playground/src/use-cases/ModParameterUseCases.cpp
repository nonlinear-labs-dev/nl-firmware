#include "ModParameterUseCases.h"
#include <presets/EditBuffer.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <presets/recall/RecallParameter.h>
#include <nltools/Assert.h>
#include <libundo/undo/Scope.h>

ModParameterUseCases::ModParameterUseCases(const ParameterId& id, EditBuffer* eb)
    : m_modParam { eb->findAndCastParameterByID<ModulateableParameter>(id) }
{
  nltools_detailedAssertAlways(m_modParam != nullptr,
                               "Id: " + id.toString() + " could not be found / casted to modulateable parameter");
}

void ModParameterUseCases::undoableRecallMCPos()
{
  if(auto mc = m_modParam->getMacroControl())
  {
    mc->undoableRecallFromPreset();
    m_modParam->onChange(UpdateDocumentContributor::ChangeFlags::Generic);
  }
}

void ModParameterUseCases::recallMCAmount()
{
  if(!m_modParam->isModAmountChanged())
    return;

  auto& scope = m_modParam->getUndoScope();
  if(auto original = m_modParam->getOriginalParameter())
  {

    auto name = m_modParam->getGroupAndParameterName();
    auto transactionScope = scope.startTransaction("Recall MC Amount from Preset for '%0'", name);
    auto transaction = transactionScope->getTransaction();
    m_modParam->setModulationAmount(transaction, original->getRecallModulationAmount());
  }
}
