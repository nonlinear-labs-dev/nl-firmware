#pragma once
#include <nltools/nlohmann/json.hpp>

namespace nltools
{
  namespace msg
  {
    class API
    {
     public:
      struct ClientConnection
      {
        ClientConnection() = default;
        virtual ~ClientConnection() = default;
      };

      enum class Backend
      {
        Websocket,
        Mock
      };

      virtual ~API() = default;
      virtual void send(ClientConnection *c, const nlohmann::json &msg) = 0;
      virtual void sendAll(const nlohmann::json &msg) = 0;
      virtual void sendAllUpdating(const nlohmann::json &msg) = 0;
      [[nodiscard]] virtual bool hasClients() const = 0;
    };
  }
}