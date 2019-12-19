#include <testing/TestHelper.h>
#include <third-party/include/catch.hpp>
#include <device-settings/AutoLoadSelectedPreset.h>


SCENARIO("'toggle' Load Mode Setting")
{
  auto setting = Application::get().getSettings()->getSetting<AutoLoadSelectedPreset>();
  auto getValue = [&]() {
    return setting->get();
  };

  REQUIRE(setting);

  setting->set(LoadPresetMode::DirectLoad);

  WHEN("Sound type Single")
  {
    AutoLoadSelectedPreset::cycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadPresetMode::PresetSelect);
    AutoLoadSelectedPreset::cycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadPresetMode::DirectLoad);
  }

  WHEN("Sound type Dual")
  {
    AutoLoadSelectedPreset::cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadPresetMode::PartSelect);
    AutoLoadSelectedPreset::cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadPresetMode::PresetSelect);
    AutoLoadSelectedPreset::cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadPresetMode::DirectLoad);
  }
}
