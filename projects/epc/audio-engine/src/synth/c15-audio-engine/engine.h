#pragma once

#include <c15_config.h>
#include "ParallelData.h"

namespace Engine
{

using ScalarValue = float;

using PolyValue = ParallelData<ScalarValue, C15::Config::local_polyphony>;
using PolyInt = ParallelData<int32_t, C15::Config::local_polyphony>;

using VoiceId = uint8_t;

}
