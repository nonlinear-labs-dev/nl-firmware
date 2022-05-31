#include <nltools/threading/Throttler.h>

Throttler::Throttler(Expiration::Duration maxDelay)
    : m_expiration([this] { delayedCallback(); })
    , m_maxDelay(maxDelay)
{
}

void Throttler::doTask(Throttler::Task&& task)
{
  m_isPending = true;
  m_task = std::move(task);

  if(!m_expiration.isPending())
    m_expiration.refresh(std::chrono::milliseconds(1));
}

Throttler::~Throttler()
{
  m_expiration.cancel();
}

bool Throttler::isPending() const
{
  return m_isPending;
}

void Throttler::delayedCallback()
{
  m_isPending = false;

  if(Task task = std::move(m_task))
  {
    task();
    m_expiration.refresh(m_maxDelay);
  }
}