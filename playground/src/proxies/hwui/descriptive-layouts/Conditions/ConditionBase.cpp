#include "ConditionBase.h"

ConditionBase::ConditionBase() = default;

ConditionBase::~ConditionBase() = default;

void ConditionBase::connect(ConditionBase::CB cb)
{
  m_onConditionChanged = cb;
}

bool ConditionBase::update()
{
  auto res = check();
  if(m_cache != res)
  {
    m_cache = res;
    onConditionChanged();
  }
  return res;
}

bool ConditionBase::get() const
{
  return m_cache;
}

void ConditionBase::onConditionChanged() const
{
  if(m_onConditionChanged)
    m_onConditionChanged();
}
