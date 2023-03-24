#include "SyncedItem.h"
#include "SyncMaster.h"
#include <tools/Uuid.h>

SyncedItem::SyncedItem(SyncMaster &master)
    : SyncedItem(master, Uuid().raw())
{
}

SyncedItem::SyncedItem(SyncMaster &master, const Topic &topic)
    : m_master(master)
    , m_topic(topic)
{
  m_master.add(this);
}

SyncedItem::~SyncedItem()
{
  m_master.remove(this);
}

void SyncedItem::setTopic(const Topic &topic)
{
  m_master.remove(this);
  m_topic = topic;
  m_master.add(this);
}

const SyncedItem::Topic &SyncedItem::getTopic() const
{
  return m_topic;
}

const nlohmann::json &SyncedItem::getState() const
{
  return m_state;
}

bool SyncedItem::updateState()
{
  if(std::exchange(m_dirty, false))
  {
    auto newState = serialize();

    if(newState != getState())
    {
      m_state = std::move(newState);
      return true;
    }
  }
  return false;
}

bool SyncedItem::isDirty() const
{
  return m_dirty;
}

void SyncedItem::setDirty()
{
  if(!std::exchange(m_dirty, true))
    m_master.addDirty(this);
}

void SyncedItem::resetDirty()
{
  if(std::exchange(m_dirty, false))
    m_master.removeDirty(this);
}

void SyncedItem::suspend()
{
  m_master.remove(this);
}

void SyncedItem::resume()
{
  m_master.add(this);
}
