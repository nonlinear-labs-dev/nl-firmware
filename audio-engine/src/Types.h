#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <asoundlib.h>
#include <iostream>

using Sample = float;
using SamplePosition = uint64_t;
using MidiEvent = snd_seq_event_t;

struct SampleFrame
{
  Sample left;
  Sample right;
};

static const auto c_numVoices = 1024;

#define TRACE(fn, ln) std::cerr << fn << ": " << ln << std::endl
#define TRACE_HERE() TRACE(__PRETTY_FUNCTION__, __LINE__)
