#include "RecursionGuard.h"

std::shared_ptr<RecursionGuard::Lock> RecursionGuard::lock()
{
  if(m_lock.lock())
    return nullptr;

  std::shared_ptr<RecursionGuard::Lock> ret(new Lock());
  m_lock = ret;
  return ret;
}

bool RecursionGuard::isLocked() const
{
  return m_lock.use_count() > 0;
}
