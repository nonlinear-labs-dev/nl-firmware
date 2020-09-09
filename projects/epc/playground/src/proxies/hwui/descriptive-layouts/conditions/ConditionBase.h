#pragma once

#include <atomic>
#include <functional>

class ConditionBase
{
 public:
  ConditionBase();
  virtual ~ConditionBase();

  using CB = std::function<void()>;
  void connect(CB cb);
  bool update();
  bool get() const;

 protected:
  void onConditionChanged() const;
  virtual bool check() const = 0;

  std::atomic_bool m_cache { false };
  CB m_onConditionChanged;
};
