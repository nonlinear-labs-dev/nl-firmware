#include "SyncMaster.h"
#include "SyncedItem.h"
#include <nltools/messaging/WebSocketJsonAPI.h>
#include <sync/JsonAdlSerializers.h>

using namespace std::chrono_literals;
using namespace nltools::msg;

namespace nlohmann
{
  template <> struct adl_serializer<SyncedItem *>
  {
    static void to_json(json &j, const SyncedItem *i)
    {
      j = { { "topic", i->getTopic() }, { "state", i->getState() } };
    }
  };
}

SyncMaster::SyncMaster()
    : m_api(std::make_unique<WebSocketJsonAPI>(SYNCJS_WEBSOCKET_PORT, [this](auto in) { return api(in); }))
    , m_throttler(5ms)
{
}

SyncMaster::~SyncMaster() = default;

nlohmann::json SyncMaster::api(const nlohmann::json &in)
{
  if(in.is_object())
  {
    handleDirty();

    auto obj = in.get<nlohmann::json::object_t>();
    auto it = obj.begin();
    auto name = it->first;
    auto args = it->second;

    if(name == "sync")
      return m_items;
  }
  return {};
}

void SyncMaster::add(SyncedItem *item)
{
  m_items.push_back(item);
  item->setDirty();
}

void SyncMaster::remove(SyncedItem *i)
{
  m_items.erase(std::find(m_items.begin(), m_items.end(), i));
  i->resetDirty();
  nlohmann::json r;
  r.push_back({ { "topic", i->getTopic() }, { "state", nullptr } });
  m_api->sendAll(r);
}

void SyncMaster::addDirty(SyncedItem *item)
{
  m_dirty.push_back(item);
  m_throttler.doTask([this] { handleDirty(); });
}

void SyncMaster::removeDirty(SyncedItem *item)
{
  m_dirty.erase(std::find(m_dirty.begin(), m_dirty.end(), item));
}

void SyncMaster::handleDirty()
{
  auto cp(std::move(m_dirty));
  auto newEnd = std::remove_if(cp.begin(), cp.end(), [](auto i) { return !i->updateState(); });
  cp.erase(newEnd, cp.end());
  if(!cp.empty())
    m_api->sendAll(cp);
}
