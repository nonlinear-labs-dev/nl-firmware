#pragma once

#include "nltools/enums/EnumTools.h"
#include <limits>
#include <type_traits>
#include <glibmm/ustring.h>

ENUM(SoundType, int8_t, Single, Split, Layer, Invalid);
ENUM(MacroControls, int8_t, NONE = 0, MC1 = 1, MC2 = 2, MC3 = 3, MC4 = 4, MC5 = 5, MC6 = 6, NUM_CHOICES = 7);
ENUM(VoiceGroup, int8_t, I, II, Global, NumGroups);
ENUM(RibbonTouchBehaviour, int8_t, ABSOLUTE, RELATIVE, NUM_TOUCH_BEHAVIOURS);
ENUM(RibbonReturnMode, int8_t, STAY = 0, RETURN = 1, NUM_RETURN_MODES = 2);
ENUM(PedalModes, int8_t, STAY = 0, RETURN_TO_ZERO = 1, RETURN_TO_CENTER = 2, NUM_PEDAL_MODES = 3);
ENUM(ReturnMode, int8_t, None, Center, Zero);

template <typename T> inline std::enable_if_t<std::is_floating_point_v<T>, bool> differs(T a, T b)
{
  return std::abs(a - b) > std::numeric_limits<T>::epsilon();
}

namespace Gio
{
  class File;
  class FileInfo;
  class InputStream;
  class AsyncResult;
  class Cancellable;
  class DataInputStream;
  class DataOutputStream;
  class ZlibCompressor;
  class FileMonitor;
}

namespace Glib
{
  class Bytes;
  class IOChannel;

  template <class T_CppObject> class RefPtr;
}
