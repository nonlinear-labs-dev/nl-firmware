#include <nltools/threading/Threading.h>
#include <nltools/logging/Log.h>
#include <thread>

void nltools::threading::setThisThreadPrio(nltools::threading::Priority p)
{
  switch(p)
  {
    case Priority::Realtime:
    {
      sched_param p { sched_get_priority_max(SCHED_FIFO) };
      if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &p))
        nltools::Log::warning("set thread prio failed. Consider 'sudo setcap 'cap_sys_nice=eip' <application>'");
      break;
    }

    case Priority::AlmostRealtime:
    {
      sched_param p { sched_get_priority_max(SCHED_FIFO) - 1 };
      if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &p))
        nltools::Log::warning("set thread prio failed. Consider 'sudo setcap 'cap_sys_nice=eip' <application>'");
      break;
    }

    default:
      break;
  }
}
