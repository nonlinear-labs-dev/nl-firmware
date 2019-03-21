#pragma once

class AudioOutput
{
 public:
  AudioOutput();

  virtual void start();
  virtual void stop();

  virtual double getPerformance() const = 0;
};
