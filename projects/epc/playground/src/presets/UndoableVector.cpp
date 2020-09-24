#include "UndoableVector.h"
#include <device-settings/CrashOnError.h>
#include <device-settings/Settings.h>
#include <Application.h>

bool UndoableVectorBase::shouldCrashOnError() const
{
  return Application::get().getSettings()->getSetting<CrashOnError>()->get();
}

bool UndoableVectorBase::select(UNDO::Transaction *transaction, const Uuid &uuid)
{
  Checker checker(this);
  if(m_selection != uuid)
  {
    transaction->addSimpleCommand([this, swap = UNDO::createSwapData(uuid)](auto) {
      Checker checker(this);
      auto old = m_selection;
      swap->swapWith(m_selection);
      invalidate();
      invalidate(old);
      invalidate(m_selection);
    });
    return true;
  }
  return false;
}

UndoableVectorBase::Checker::Checker(UndoableVectorBase *p)
    : m_p(p)
{
  m_p->check();
}

UndoableVectorBase::Checker::~Checker()
{
  m_p->check();
}
