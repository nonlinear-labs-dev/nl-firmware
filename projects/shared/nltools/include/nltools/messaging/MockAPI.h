#pragma once
#include <nltools/messaging/API.h>

namespace nltools
{
  namespace msg
  {
    class MockAPI : public API
    {
     public:
      void sendAll(const nlohmann::json& msg) override
      {
      }

      void sendAllUpdating(const nlohmann::json& msg) override
      {
      }

      [[nodiscard]] bool hasClients() const override
      {
        return false;
      }
    };
  }
}