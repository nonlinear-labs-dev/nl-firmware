#pragma once

#include "Types.h"
#include <nltools/nlohmann/json.hpp>
#include <memory>
#include <sigc++/connection.h>

class FlacFrameStorage;
class RecorderInput;
class RecorderOutput;
class NetworkServer;

namespace nltools
{
  namespace msg
  {
    class WebSocketJsonAPI;
  }
}

class Recorder
{
 public:
  Recorder(int sr);
  ~Recorder();

  void process(SampleFrame* target, size_t numFrames);

  RecorderInput* getInput() const;
  RecorderOutput* getOutput() const;
  FlacFrameStorage* getStorage() const;

  nlohmann::json api(const nlohmann::json& msg);

  void notifyStateChange();
  
private:
  nlohmann::json generateInfo() const;
  nlohmann::json prepareDownload(FrameId begin, FrameId end) const;
  nlohmann::json queryFrames(FrameId begin, FrameId end) const;

  std::unique_ptr<FlacFrameStorage> m_storage;
  std::unique_ptr<RecorderInput> m_in;
  std::unique_ptr<RecorderOutput> m_out;
  std::unique_ptr<nltools::msg::WebSocketJsonAPI> m_api;
  std::unique_ptr<NetworkServer> m_http;
  sigc::connection m_settingConnection;
};
