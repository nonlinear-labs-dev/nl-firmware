#pragma once

#include "Types.h"
#include <asoundlib.h>
#include <functional>
#include <thread>

class AudioOutput
{
 public:
  using Callback = std::function<void(SampleFrame *target, size_t numFrames)>;

  AudioOutput(const std::string &deviceName, Callback cb);
  ~AudioOutput();

  void start();
  void stop();

  std::chrono::nanoseconds getLatency() const;
  double getPerformance() const;

  void prioritizeThread();

 private:
  void open(const std::string &deviceName);

  void close();
  void doBackgroundWork();
  void handleWriteError(snd_pcm_sframes_t result);
  void setThreadAffinity();
  void playback(SampleFrame *frames, size_t numFrames);
  template <typename T> snd_pcm_sframes_t playbackIntLE(const SampleFrame *frames, size_t numFrames);
  snd_pcm_sframes_t playbackF32(SampleFrame *frames, size_t numFrames);
  snd_pcm_sframes_t playbackInt24LE(const SampleFrame *frames, size_t numFrames);

  Callback m_cb;
  snd_pcm_t *m_handle = nullptr;
  std::thread m_bgThread;
  bool m_run = true;
  unsigned int m_latency = 0;
  snd_pcm_uframes_t m_framesPerPeriod = 0;
  snd_pcm_uframes_t m_ringBufferFrames = 0;
  snd_pcm_uframes_t m_framesProcessed = 0;
  snd_pcm_format_t m_format = SND_PCM_FORMAT_S32_LE;

  double m_performance = 0;
};
