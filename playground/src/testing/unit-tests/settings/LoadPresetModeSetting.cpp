#include <testing/TestHelper.h>
#include <third-party/include/catch.hpp>
#include <device-settings/LoadPresetSetting.h>

SCENARIO("'toggle' Load Mode Setting")
{
  auto setting = Application::get().getSettings()->getSetting<LoadPresetSetting>();
  auto getValue = [&]() { return setting->get(); };

  REQUIRE(setting);

  setting->set(LoadMode::DirectLoad);

  WHEN("Sound type Single")
  {
    LoadPresetSetting::cycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadMode::Select);
    LoadPresetSetting::cycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadMode::DirectLoad);
  }

  WHEN("Sound type Dual")
  {
    LoadPresetSetting::cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::LoadToPart);
    LoadPresetSetting::cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::Select);
    LoadPresetSetting::cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::DirectLoad);
  }
}
