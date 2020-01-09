#pragma once

#include <memory>

class ScopedGuard
{
 public:
  ScopedGuard();
  virtual ~ScopedGuard();

  class Lock
  {
   public:
    Lock()
    {
    }
  };

  std::shared_ptr<Lock> lock();
  bool isLocked() const;

 private:
  std::weak_ptr<Lock> m_currentLock;
};
