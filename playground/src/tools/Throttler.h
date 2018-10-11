#pragma once

#include "playground.h"
#include "Expiration.h"

class Throttler
{
 public:
  using Task = function<void()>;

  Throttler(Expiration::Duration maxDelay);

  void doTask(Task&& task);
  bool isPending() const;

 private:
  void delayedCallback();

  Expiration m_expiration;
  Expiration::Duration m_maxDelay;
  Task m_task;
  bool m_isPending = false;
};
