#include "third-party/include/catch.hpp"
#include "testing/TestHelper.h"
#include "MockPresetStorage.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <thread>

inline Layout* getLayoutPtr()
{
  return Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().getLayout().get();
};

TEST_CASE("Sound Screen toggle VoiceGroup does not show Paramerter Screen")
{
  std::atomic_bool finished{ false };

  std::thread hwuiUpdate{ [&]() {
    while(!finished)
    {
      TestHelper::MainLoop::iterateLoopOnce();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  } };

  auto hwui = Application::get().getHWUI();

  hwui->setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });

  REQUIRE(dynamic_cast<ParameterLayout2*>(getLayoutPtr()) == nullptr);

  finished = true;
  hwuiUpdate.join();
}

TEST_CASE("HWUI Voice Group Selection")
{
  MockPresetStorage presets;

  auto hwui = Application::get().getHWUI();

  auto eb = TestHelper::getEditBuffer();
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
    hwui->setCurrentVoiceGroup(VoiceGroup::II);
    REQUIRE(hwui->getCurrentVoiceGroup() == VoiceGroup::II);
  }

  SECTION("Reset Voice Group Selection to I on Preset Load")
  {
    {
      auto scope = TestHelper::createTestScope();
      hwui->setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });

      REQUIRE(dynamic_cast<PresetManagerLayout*>(hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get())
              != nullptr);

      REQUIRE(hwui->getCurrentVoiceGroup() == VoiceGroup::II);
      eb->undoableLoad(scope->getTransaction(), presets.getSplitPreset());
    }

    REQUIRE(TestHelper::MainLoop::hasPending());

    while(TestHelper::MainLoop::iterateLoopOnce())
      ;

    REQUIRE_FALSE(TestHelper::MainLoop::hasPending());

    REQUIRE(hwui->getCurrentVoiceGroup() == VoiceGroup::I);
    REQUIRE(dynamic_cast<PresetManagerLayout*>(hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get())
            != nullptr);
  }
}