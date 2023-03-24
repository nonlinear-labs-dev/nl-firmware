#include "SyncMaster.h"
#include "SyncedItem.h"
#include <sync/JsonAdlSerializers.h>
#include <nltools/messaging/MockAPI.h>
#include <nltools/messaging/WebSocketJsonAPI.h>
#include <Application.h>

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

SyncMaster::SyncMaster(nltools::msg::API::Backend backend)
    : m_api(createAPI(backend))
    , m_throttler(Application::get().getMainContext(), 5ms)
{
}

SyncMaster::~SyncMaster() = default;

std::unique_ptr<SyncMaster::tAPI> SyncMaster::createAPI(SyncMaster::tAPI::Backend backend)
{
  switch(backend)
  {
    case tAPI::Backend::Websocket:
      return std::make_unique<WebSocketJsonAPI>(Application::get().getMainContext(), SYNCJS_WEBSOCKET_PORT,
                                                [this](const auto &client, auto in) { return api(client, in); });
    default:
    case tAPI::Backend::Mock:
      return std::make_unique<MockAPI>();
  }
}

nlohmann::json SyncMaster::api(const ClientPtr &client, const nlohmann::json &in)
{
  if(in.is_object())
  {
    handleDirty();

    auto obj = in.get<nlohmann::json::object_t>();
    auto it = obj.begin();
    auto name = it->first;
    auto args = it->second;

    if(name == "sub")
    {
      try
      {
        std::string topic = args;
        m_subscribers[topic].push_back(client);

        auto mapIt = std::find_if(m_items.begin(), m_items.end(), [&](auto &a) { return a->getTopic() == topic; });
        if(mapIt != m_items.end())
          return *mapIt;
      }
      catch(...)
      {
      }
    }
  }
  return {};
}

void SyncMaster::add(SyncedItem *item)
{
  m_items.insert(item);
  item->setDirty();
}

void SyncMaster::remove(SyncedItem *i)
{
  m_items.erase(i);
  i->resetDirty();

  auto topic = i->getTopic();
  nlohmann::json r = { { "topic", topic }, { "state", nullptr } };

  if(auto it = m_subscribers.find(topic); it != m_subscribers.end())
  {
    for(auto &s : it->second)
      if(auto p = s.lock())
        m_api->send(p.get(), r);

    m_subscribers.erase(it);
  }
}

void SyncMaster::addDirty(SyncedItem *item)
{
  m_dirty.push_back(item);
  m_throttler.doTask([this] { handleDirty(); });
}

void SyncMaster::removeDirty(SyncedItem *item)
{
  m_dirty.erase(std::remove(m_dirty.begin(), m_dirty.end(), item), m_dirty.end());
}

void SyncMaster::handleDirty()
{
  auto cp(std::move(m_dirty));
  m_dirty.clear();

  auto newEnd = std::remove_if(cp.begin(), cp.end(), [](auto i) { return !i->updateState(); });
  cp.erase(newEnd, cp.end());

  for(const auto &dirtyItem : cp)
  {
    if(auto it = m_subscribers.find(dirtyItem->getTopic()); it != m_subscribers.end())
    {
      auto &clients = it->second;

      clients.erase(std::remove_if(clients.begin(), clients.end(),
                                   [&](auto &client)
                                   {
                                     if(auto p = client.lock())
                                     {
                                       m_api->send(p.get(), dirtyItem);
                                       return false;
                                     }
                                     return true;
                                   }),
                    clients.end());
    }
  }
}
