#pragma once

#include <nltools/Uncopyable.h>
#include <chrono>
#include <functional>
#include <sigc++/connection.h>

class Expiration : public Uncopyable
{
 public:
  using Callback = std::function<void()>;
  using Duration = std::chrono::steady_clock::duration;

  Expiration(Callback cb = Callback(), Duration d = Duration::zero(), int priority = 0);
  ~Expiration();

  void setCallback(Callback cb);
  void refresh(Duration d, int priority = 0);
  bool isPending() const;
  void cancel();

 private:
  bool doCallback();

  Callback m_cb;
  sigc::connection m_timeout;
};
