#include <testing/TestHelper.h>
#include <third-party/include/catch.hpp>
#include <device-settings/LoadModeSetting.h>
#include <device-settings/Settings.h>

SCENARIO("'toggle' Load Mode Setting")
{
  auto setting = Application::get().getSettings()->getSetting<LoadModeSetting>();
  auto getValue = [&]() { return setting->get(); };

  REQUIRE(setting);

  setting->set(LoadMode::DirectLoad);

  WHEN("Sound type Single")
  {
    setting->cycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadMode::Select);
    setting->cycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadMode::DirectLoad);
  }

  WHEN("Sound type Dual")
  {
    setting->cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::LoadToPart);
    setting->cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::Select);
    setting->cycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::DirectLoad);
  }

  WHEN("Sound type Single (anit)")
  {
    setting->antiCycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadMode::Select);
    setting->antiCycleForSoundType(SoundType::Single);
    REQUIRE(getValue() == LoadMode::DirectLoad);
  }

  WHEN("Sound type Dual (anit)")
  {
    setting->antiCycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::Select);
    setting->antiCycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::LoadToPart);
    setting->antiCycleForSoundType(SoundType::Layer);
    REQUIRE(getValue() == LoadMode::DirectLoad);
  }
}
