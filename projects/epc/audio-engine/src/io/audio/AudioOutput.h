#pragma once

#include <atomic>
#include <stdlib.h>

class AudioOutput
{
 public:
  AudioOutput();

  struct Performance
  {
    double overallMin = 0;
    double min = 0;
    double sum = 0;
    size_t num = 0;
    double max = 0;
    double overallMax = 0;
  };

  virtual void start();
  virtual void stop();

  Performance exhaustPerformance();
  void resetPerformance();

  size_t getNumUnderruns() const;

 protected:
  void reportPerformanceRatio(double ratio);
  void reportBufferUnderrun();

 private:
  Performance m_performance;
  std::atomic<bool> m_resetPerformance = true;
  std::atomic<bool> m_exhaustPerformance = true;

  size_t m_numUnderruns = 0;
};
