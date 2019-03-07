#include "AudioOutput.h"
#include "main.h"
#include "Options.h"
#include <iostream>
#include <algorithm>

AudioOutput::AudioOutput(const std::string& deviceName, Callback cb)
    : m_cb(cb)
{
  open(deviceName);
}

AudioOutput::~AudioOutput()
{
  close();
}

std::chrono::nanoseconds AudioOutput::getLatency() const
{
  return std::chrono::microseconds(static_cast<uint64_t>(m_latency));
}

double AudioOutput::getPerformance() const
{
  return m_performance * 100;
}

void AudioOutput::close()
{
  if(auto h = std::exchange(m_handle, nullptr))
    snd_pcm_close(h);
}

void AudioOutput::open(const std::string& deviceName)
{
  snd_pcm_open(&m_handle, deviceName.c_str(), SND_PCM_STREAM_PLAYBACK, 0);

  snd_pcm_hw_params_t* hwparams = NULL;
  snd_pcm_sw_params_t* swparams = NULL;

  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);

  unsigned int sampleRate = getOptions()->getSampleRate();
  unsigned int periods = 2;

  auto timePerPeriod = 1.0 * getOptions()->getLatency() / (periods / 2);

  m_framesPerPeriod = timePerPeriod * sampleRate / 1000;
  m_ringBufferFrames = periods * m_framesPerPeriod;

  snd_pcm_hw_params_any(m_handle, hwparams);
  snd_pcm_hw_params_set_access(m_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
  snd_pcm_hw_params_set_format(m_handle, hwparams, SND_PCM_FORMAT_S32_LE);
  snd_pcm_hw_params_set_channels(m_handle, hwparams, 2);
  snd_pcm_hw_params_set_rate_near(m_handle, hwparams, &sampleRate, 0);
  snd_pcm_hw_params_set_periods(m_handle, hwparams, periods, 0);
  snd_pcm_hw_params_set_period_size_near(m_handle, hwparams, &m_framesPerPeriod, 0);
  snd_pcm_hw_params_set_buffer_size_near(m_handle, hwparams, &m_ringBufferFrames);
  snd_pcm_hw_params(m_handle, hwparams);
  snd_pcm_sw_params_current(m_handle, swparams);
  snd_pcm_sw_params(m_handle, swparams);

  snd_pcm_hw_params_get_period_time(hwparams, &m_latency, 0);
  snd_pcm_hw_params_get_periods(hwparams, &periods, 0);

  m_latency *= periods / 2;

  std::cout << "Alsa Frames per Period: " << m_framesPerPeriod << std::endl;
  std::cout << "Alsa Periods: " << periods << std::endl;
  std::cout << "Midi2Audio latency is: " << m_latency / 1000 << "ms." << std::endl;
}

void AudioOutput::start()
{
  m_run = true;
  m_bgThread = std::thread([=]() { doBackgroundWork(); });
}

void AudioOutput::stop()
{
  m_run = false;

  if(m_bgThread.joinable())
    m_bgThread.join();
}

void AudioOutput::prioritizeThread()
{
  struct sched_param param;
  param.sched_priority = 50;

  if(auto r = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param))
    std::cerr << "Could not set thread priority - consider 'sudo setcap 'cap_sys_nice=eip' <application>'" << std::endl;
}

void AudioOutput::setThreadAffinity()
{
  int coreID = 0;

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(coreID, &set);
  if(sched_setaffinity(0, sizeof(cpu_set_t), &set) < 0)
    std::cerr << "Could not set thread affinity" << std::endl;
}

void AudioOutput::doBackgroundWork()
{
  prioritizeThread();
  setThreadAffinity();

  snd_pcm_prepare(m_handle);

  auto prefillFrames = m_ringBufferFrames;
  SampleFrame prefillAudio[prefillFrames];
  std::fill(prefillAudio, prefillAudio + prefillFrames, SampleFrame{});

  snd_pcm_start(m_handle);
  playback(prefillAudio, prefillFrames);

  const auto framesPerCallback = m_framesPerPeriod;

  SampleFrame audio[framesPerCallback];
  std::fill(audio, audio + framesPerCallback, SampleFrame{});

  while(m_run)
  {
    auto startDSP = std::chrono::high_resolution_clock::now();
    m_cb(audio, m_framesPerPeriod);
    auto endDSP = std::chrono::high_resolution_clock::now();
    playback(audio, framesPerCallback);
    auto endPlayback = std::chrono::high_resolution_clock::now();

    auto diffDSP = endDSP - startDSP;
    auto diffAll = endPlayback - startDSP;
    auto ratio = 1.0 * diffDSP / diffAll;
    auto filterCoeff = 0.98;
    m_performance = filterCoeff * m_performance + (1 - filterCoeff) * ratio;
  }
}

void AudioOutput::playback(const SampleFrame* frames, size_t numFrames)
{
  struct Converted
  {
    int32_t left;
    int32_t right;
  };

  Converted converted[numFrames];

  for(size_t f = 0; f < numFrames; f++)
  {
    for(size_t c = 0; c < 2; c++)
    {
      converted[f].left = std::min(1.0f, 0.5f * frames[f].left) * std::numeric_limits<int32_t>::max();
      converted[f].right = std::min(1.0f, 0.5f * frames[f].right) * std::numeric_limits<int32_t>::max();
    }
  }

  auto result = snd_pcm_writei(m_handle, &converted, numFrames);

  if(static_cast<snd_pcm_uframes_t>(result) != numFrames)
    handleWriteError(result);
  else
    m_framesProcessed += numFrames;
}

void AudioOutput::handleWriteError(snd_pcm_sframes_t result)
{
  if(result < 0)
  {
    if(getOptions()->areXRunsFatal())
    {
      std::stringstream str;
      str << "XRun at frame position " << m_framesProcessed << std::endl;
      throw std::runtime_error(str.str());
    }

    if(auto recoverResult = snd_pcm_recover(m_handle, result, 1))
    {
      std::cerr << "Could not recover:" << recoverResult << std::endl;
    }
    else
    {
      std::cerr << "recovered from x-run" << std::endl;
      snd_pcm_start(m_handle);
    }
  }
}
