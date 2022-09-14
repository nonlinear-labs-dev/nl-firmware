#include "DelayedJob.h"
#include <Application.h>

DelayedJob::DelayedJob(int maxDelay, tJob job)
    : m_maxDelay(maxDelay)
    , m_job(job)
{
}

DelayedJob::~DelayedJob()
{
  m_timeout.disconnect();
}

void DelayedJob::trigger()
{
  if(m_timeout.empty())
  {
    m_timeout = Application::get().getMainContext()->signal_timeout().connect(
        sigc::mem_fun(this, &DelayedJob::onTimeout), m_maxDelay);
  }
}

bool DelayedJob::onTimeout()
{
  m_timeout.disconnect();
  m_job();
  return false;
}
