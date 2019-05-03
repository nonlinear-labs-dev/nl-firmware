#include <tools/ScopedGuard.h>

ScopedGuard::ScopedGuard()
{
}

ScopedGuard::~ScopedGuard()
{
}

std::shared_ptr<ScopedGuard::Lock> ScopedGuard::lock()
{
  if(isLocked())
    return nullptr;

  std::shared_ptr<ScopedGuard::Lock> ptr(new Lock());
  m_currentLock = ptr;
  return ptr;
}

bool ScopedGuard::isLocked() const
{
  return m_currentLock.lock() != nullptr;
}
