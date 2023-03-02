#pragma once

#include <nltools/nlohmann/json.hpp>
#include <memory>
#include <optional>

class SyncMaster;

class SyncedItem
{
 public:
  using Topic = std::string;
  SyncedItem(SyncMaster &master);
  SyncedItem(SyncMaster &master, const Topic &topic);
  virtual ~SyncedItem();

  void setTopic(const Topic &topic);
  const Topic &getTopic() const;

  const nlohmann::json &getState() const;
  bool updateState();
  bool isDirty() const;
  void setDirty();
  void resetDirty();

  void suspend();
  void resume();

 protected:
  virtual nlohmann::json serialize() const = 0;

 private:
  SyncMaster &m_master;
  Topic m_topic;
  nlohmann::json m_state;
  bool m_dirty = false;
};
