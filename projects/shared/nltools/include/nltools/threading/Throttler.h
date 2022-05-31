#pragma once

#include <nltools/threading/Expiration.h>

class Throttler
{
 public:
  using Task = std::function<void()>;

  explicit Throttler(Expiration::Duration maxDelay);
  ~Throttler();

  void doTask(Task&& task);
  bool isPending() const;

 private:
  void delayedCallback();

  Expiration m_expiration;
  Expiration::Duration m_maxDelay;
  Task m_task;
  bool m_isPending = false;
};
