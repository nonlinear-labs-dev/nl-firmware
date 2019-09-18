#pragma once
#include <atomic>
#include <glibmm/ustring.h>

class ConditionBase
{
 public:
 #warning"adlerauge (name luegt) vlt update()?"
  bool get() const
  {
    auto res = check();
    if(m_cache != res)
    {
      m_cache = res;
      onConditionChanged();
    }
    return res;
  }

 protected:
  void onConditionChanged() const;
  virtual bool check() const = 0;
  mutable std::atomic_bool m_cache{ false };
};
