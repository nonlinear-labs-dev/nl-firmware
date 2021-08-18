#pragma once

#include <nltools/nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <nltools/threading/Throttler.h>
#include <nltools/messaging/API.h>

namespace nltools::msg
{
  class WebSocketJsonAPI;
}

class SyncedItem;

class SyncMaster
{
 public:
  typedef nltools::msg::API tAPI;
  explicit SyncMaster(tAPI::Backend api);
  ~SyncMaster();

  void add(SyncedItem *item);
  void remove(SyncedItem *item);
  void addDirty(SyncedItem *item);
  void removeDirty(SyncedItem *item);

 private:
  using ClientPtr = std::weak_ptr<nltools::msg::WebSocketJsonAPI::ClientConnection>;
  nlohmann::json api(ClientPtr client, const nlohmann::json &in);
  void handleDirty();

  std::unique_ptr<tAPI> createAPI(tAPI::Backend api);

  std::unique_ptr<tAPI> m_api;
  std::vector<SyncedItem *> m_items;
  std::vector<SyncedItem *> m_dirty;
  Throttler m_throttler;

  std::map<Topic, std::vector<ClientPtr>> m_subscribers;
};
