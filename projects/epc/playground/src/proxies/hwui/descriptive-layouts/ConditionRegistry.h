#pragma once
#include <functional>
#include <memory>
#include <proxies/hwui/descriptive-layouts/conditions/ConditionBase.h>
#include <tools/Signal.h>
#include <map>

class ConditionRegistry : public sigc::trackable
{
 public:
  typedef ConditionBase* tCondition;
  tCondition getCondition(const std::string& key);
  static ConditionRegistry& get();
  sigc::connection onChange(const std::function<void()>& cb);
  void lock();
  void unlock();

 private:
  ConditionRegistry();
  void onConditionChanged();
  bool isLocked();

  int locks = 0;
  Signal<void> m_signal;
  std::map<std::string, std::unique_ptr<ConditionBase>> m_theConditionMap;

  friend class ConditionBase;
};
