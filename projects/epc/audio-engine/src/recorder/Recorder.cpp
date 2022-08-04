#include "Recorder.h"

#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <nltools/messaging/WebSocketJsonAPI.h>
#include <nltools/logging/Log.h>

#include "FlacFrameStorage.h"
#include "RecorderInput.h"
#include "RecorderOutput.h"
#include "NetworkServer.h"

constexpr int operator""_MB(unsigned long long const x)
{
  return 1024 * 1024 * x;
}

constexpr static auto c_flacFrameBufferSize = 500_MB;

Recorder::Recorder(int sr)
    : m_storage(std::make_unique<FlacFrameStorage>(c_flacFrameBufferSize))
    , m_in(std::make_unique<RecorderInput>(m_storage.get(), sr))
    , m_out(std::make_unique<RecorderOutput>(m_storage.get(), sr))
    , m_api(std::make_unique<nltools::msg::WebSocketJsonAPI>(Glib::MainContext::get_default(), RECORDER_WEBSOCKET_PORT,
                                                             [this](auto, const auto &msg) { return api(msg); }))
    , m_http(std::make_unique<NetworkServer>(RECORDER_HTTPSERVER_PORT, m_storage.get()))
{
  m_in->setPaused(true);
  m_settingConnection = nltools::msg::receive<nltools::msg::Setting::FlacRecorderAutoStart>(
      nltools::msg::EndPoint::AudioEngine, [this](const auto &msg) {
        if(msg.enabled)
          m_in->setPaused(false);
      });
}

Recorder::~Recorder() = default;

void Recorder::process(SampleFrame *frames, size_t numFrames)
{
  m_in->process(frames, numFrames);
  m_out->process(frames, numFrames);
}

RecorderInput *Recorder::getInput() const
{
  return m_in.get();
}

RecorderOutput *Recorder::getOutput() const
{
  return m_out.get();
}

FlacFrameStorage *Recorder::getStorage() const
{
  return m_storage.get();
}

void Recorder::notifyStateChange()
{
  nltools::msg::send(nltools::msg::EndPoint::Playground, nltools::msg::Recorder::State { m_in->isRecording() });
}

nlohmann::json Recorder::api(const nlohmann::json &msg)
{
  if(msg.is_object())
  {
    auto obj = msg.get<nlohmann::json::object_t>();
    auto it = obj.begin();
    auto name = it->first;
    auto args = it->second;

    if(name == "toggle-recording")
    {
      m_in->togglePause();
      notifyStateChange();
    }
    else if(name == "set-playback-position")
    {
      m_out->setPlayPos(args.at("frameId"));
    }
    else if(name == "start-playback")
    {
      m_out->start();
    }
    else if(name == "pause-playback")
    {
      m_out->pause();
    }
    else if(name == "get-info")
    {
      return generateInfo();
    }
    else if(name == "query-frames")
    {
      return queryFrames(args.at("begin"), args.at("end"));
    }
    else if(name == "prepare-download")
    {
      return prepareDownload(args.at("begin"), args.at("end"));
    }
    else if(name == "reset")
    {
      auto ret = m_storage->reset();
      m_out->setPlayPos(-1);
      return ret;
    }
  }
  return {};
}

nlohmann::json Recorder::generateInfo() const
{
  nlohmann::json ret;
  ret["time"] = std::chrono::system_clock::now().time_since_epoch().count();
  ret["storage"] = m_storage->generateInfo();
  ret["recorder"] = m_in->generateInfo();
  ret["player"] = m_out->generateInfo();
  return ret;
}

nlohmann::json Recorder::prepareDownload(FrameId begin, FrameId end) const
{
  nlohmann::json ret;

  auto numFrames = end - begin + 1;
  size_t numFlacBytes = 0;
  size_t numWaveBytes = 44 + numFrames * (FlacEncoder::flacFrameSize * 3 * 2);

  auto stream = m_storage->startStream(begin, end);

  stream->getFirstAndLast([&](const auto &first, const auto &last) {
    ret["range"] = { { "from", first.recordTime.time_since_epoch().count() },
                     { "to", last.recordTime.time_since_epoch().count() } };

    numFlacBytes = last.summedUpFlacMemUsage - first.summedUpFlacMemUsage + first.buffer.size();
  });

  ret["flac"] = { { "size", numFlacBytes } };
  ret["wave"] = { { "size", numWaveBytes } };
  return ret;
}

nlohmann::json Recorder::queryFrames(FrameId begin, FrameId end) const
{
  using namespace std::chrono;
  using namespace std::chrono_literals;

  constexpr nanoseconds frameLen(std::nano::den * FlacEncoder::flacFrameSize / 48000);

  auto ret = nlohmann::json::array();
  auto stream = m_storage->startStream(begin, end);

  constexpr auto invalidTime = system_clock::time_point::min();

  system_clock::time_point recordTimeOfLastFrame = invalidTime;
  uint8_t maxOfLastFrame = 0;

  auto cb = [&](const auto &f, auto isLast) {
    bool skipFrame = false;

    if(!isLast && recordTimeOfLastFrame != invalidTime)
    {
      if(f.max == maxOfLastFrame)
      {
        auto expectedTS = recordTimeOfLastFrame + frameLen;
        auto diff = duration_cast<nanoseconds>(expectedTS - f.recordTime);
        auto oneMS = 1000 * 1000;
        skipFrame = std::abs(diff.count()) < oneMS;
      }
    }

    if(!skipFrame)
      ret.push_back(f.generateInfo());

    recordTimeOfLastFrame = f.recordTime;
    maxOfLastFrame = f.max;
  };

  auto start = steady_clock::now();
  while(stream->next(cb))
  {
    auto diff = steady_clock::now() - start;
    if(diff > 50ms)
      break;
  }

  return ret;
}
