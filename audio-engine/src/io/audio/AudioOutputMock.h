#include "AudioOutput.h"

#pragma once

class AudioOutputMock : public AudioOutput
{
 public:
  AudioOutputMock();

  double getPerformance() const override;
};
