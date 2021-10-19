#pragma once
#include <device-settings/Settings.h>
#include <optional>

enum class PlaceholderIDS : int {
  Pedal1_Send = 1000,
  Pedal2_Send = 1001,
  Pedal3_Send = 1002,
  Pedal4_Send = 1003,
  Ribbon1_Send = 1004,
  Ribbon2_Send = 1005,
  Bender_Send = 1006,
  Aftertouch_Send = 1007
};

using OptRefSettings = std::optional<std::reference_wrapper<Settings>>;
