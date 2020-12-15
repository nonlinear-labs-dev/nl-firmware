#include "EditBufferUseCases.h"
#include <presets/EditBuffer.h>
#include <presets/Preset.h>
#include <parameters/Parameter.h>
#include <libundo/undo/Scope.h>

EditBufferUseCases::EditBufferUseCases(EditBuffer* eb)
    : m_editBuffer { eb }
{
}

void EditBufferUseCases::resetCustomScale()
{
  auto scope = m_editBuffer->getUndoScope().startTransaction("Reset Custom Scale");
  auto scaleGroup = m_editBuffer->getParameterGroupByID({ "Scale", VoiceGroup::Global });
  scaleGroup->undoableReset(scope->getTransaction(), Initiator::EXPLICIT_USECASE);
}

void EditBufferUseCases::undoableLoadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to)
{
  auto transactionName = preset->buildUndoTransactionTitle("Load Part " + toString(from) + " To Part " + toString(to));
  auto scope = m_editBuffer->getUndoScope().startTransaction(transactionName);
  m_editBuffer->undoableLoadToPart(scope->getTransaction(), preset, from, to);
}

void EditBufferUseCases::undoableLoad(const Preset* preset)
{
  if(m_editBuffer->getUUIDOfLastLoadedPreset() != preset->getUuid())
  {
    auto scope = m_editBuffer->getUndoScope().startTransaction(preset->buildUndoTransactionTitle("Load"));
    m_editBuffer->undoableLoad(scope->getTransaction(), preset, true);
  }
}

std::unique_ptr<ParameterUseCases> EditBufferUseCases::getUseCase(ParameterId id)
{
  return std::move(std::make_unique<ParameterUseCases>(id, m_editBuffer));
}

void EditBufferUseCases::selectParameter(const ParameterId& id)
{
  if(auto p = m_editBuffer->findParameterByID(id))
  {
    auto& undoScope = m_editBuffer->getUndoScope();
    auto groupAndParamName = p->getGroupAndParameterName();
    auto scope = undoScope.startContinuousTransaction(&p, std::chrono::hours(1), "Select '%0'", groupAndParamName);
    m_editBuffer->undoableSelectParameter(scope->getTransaction(), ParameterId(id));
  }

  //TODO: if parameter was already selected this code was called from editbuffer:
  //    auto hwui = Application::get().getHWUI();
  //    if(hwui->getFocusAndMode().mode == UIMode::Info)
  //      hwui->undoableSetFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Info));
  //    else
  //      hwui->undoableSetFocusAndMode(FocusAndMode(UIFocus::Parameters, UIMode::Select));
}

void EditBufferUseCases::selectParameter(const Parameter* p)
{
  if(p)
  {
    selectParameter(p->getID());
  }
}
