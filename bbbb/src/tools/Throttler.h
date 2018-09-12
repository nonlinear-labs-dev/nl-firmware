#pragma once

#include "bbbb.h"
#include "Expiration.h"
#include <functional>

class Throttler
{
  public:
   using Task = std::function<void()>;

   Throttler(Expiration::Duration maxDelay);

   void doTask(Task&& task);

  private:
    void delayedCallback();

    Expiration m_expiration;
    Expiration::Duration m_maxDelay;
    Task m_task;
};

