#include "Throttler.h"

Throttler::Throttler(Expiration::Duration maxDelay)
    : m_expiration(bind(&Throttler::delayedCallback, this))
    , m_maxDelay(maxDelay)
{
}

void Throttler::doTask(Throttler::Task&& task)
{
  m_task = task;

  if(!m_expiration.isPending())
  {
    m_expiration.refresh(std::chrono::milliseconds(1));
  }
}

bool Throttler::isPending() const
{
  return m_expiration.isPending();
}

void Throttler::delayedCallback()
{
  Task t;
  std::swap(t, m_task);

  if(t)
  {
    t();
    m_expiration.refresh(m_maxDelay);
  }
}
