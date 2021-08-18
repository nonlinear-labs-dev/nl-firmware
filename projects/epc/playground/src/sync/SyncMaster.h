#pragma once

#include <nltools/nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <nltools/threading/Throttler.h>

namespace nltools::msg
{
  class WebSocketJsonAPI;
}

class SyncedItem;

class SyncMaster
{
 public:
  SyncMaster();
  ~SyncMaster();

  void add(SyncedItem *item);
  void remove(SyncedItem *item);
  void addDirty(SyncedItem *item);
  void removeDirty(SyncedItem *item);

 private:
  nlohmann::json api(const nlohmann::json &in);
  void handleDirty();

  std::unique_ptr<nltools::msg::WebSocketJsonAPI> m_api;
  std::vector<SyncedItem *> m_items;
  std::vector<SyncedItem *> m_dirty;
  Throttler m_throttler;
};
