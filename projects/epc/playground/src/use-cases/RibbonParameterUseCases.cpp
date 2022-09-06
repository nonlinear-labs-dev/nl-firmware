#include <nltools/Assert.h>
#include "RibbonParameterUseCases.h"
#include <parameters/RibbonParameter.h>
#include <libundo/undo/Scope.h>

RibbonParameterUseCases::RibbonParameterUseCases(RibbonParameter* p)
    : PhysicalControlParameterUseCases{p}
    , m_param { p }
{
  nltools_assertAlways(m_param != nullptr);
}

void RibbonParameterUseCases::setReturnMode(const Glib::ustring& mode)
{
  auto scope = m_param->getUndoScope().startTransaction("Set ribbon return mode");
  m_param->undoableSetRibbonReturnMode(scope->getTransaction(), mode, Initiator::EXPLICIT_USECASE);
}

void RibbonParameterUseCases::setReturnMode(const RibbonReturnMode& mode)
{
  auto scope = m_param->getUndoScope().startTransaction("Set ribbon return mode");
  m_param->undoableSetRibbonReturnMode(scope->getTransaction(), mode, Initiator::EXPLICIT_USECASE);
}

void RibbonParameterUseCases::setTouchBehaviour(const Glib::ustring& mode)
{
  auto scope = m_param->getUndoScope().startTransaction("Set ribbon touch behaviour");
  m_param->undoableSetRibbonTouchBehaviour(scope->getTransaction(), mode);
}

void RibbonParameterUseCases::incTouchBehaviour()
{
  auto trans = m_param->getUndoScope().startTransaction("Set ribbon touch behaviour");
  m_param->undoableIncRibbonTouchBehaviour(trans->getTransaction());
}

void RibbonParameterUseCases::stepTouchBehaviour(int i)
{
  i = std::abs(i);
  auto scope = m_param->getUndoScope().startTransaction("Set ribbon touch behaviour");
  for(int it = 0; it < i; it++)
  {
    m_param->undoableIncRibbonTouchBehaviour(scope->getTransaction());
  }
}
