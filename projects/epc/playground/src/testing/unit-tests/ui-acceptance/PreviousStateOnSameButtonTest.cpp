#include <testing/TestHelper.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include "use-cases/SettingsUseCases.h"
#include <device-settings/Settings.h>
#include <device-settings/FocusAndModeSetting.h>

TEST_CASE("Test Previous Boled Focus on Button")
{
  auto hwui = Application::get().getHWUI();
  auto& editPanel = hwui->getPanelUnit().getEditPanel();
  SettingsUseCases useCases(*TestHelper::getSettings());

  useCases.setFocusAndMode({ UIFocus::Parameters, UIMode::Select, UIDetail::Init });

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::getEditBuffer()->undoableSelectParameter(scope->getTransaction(), { 0, VoiceGroup::I }, false);
  }

  editPanel.getBoled().bruteForce();

  auto getLayout = [hwui, &editPanel] { return editPanel.getBoled().getLayout().get(); };

  auto isParameterLayout = [](auto layout) { return dynamic_cast<const ParameterLayout2*>(layout) != nullptr; };

  auto isPresetLayout = [](auto layout) { return dynamic_cast<const PresetManagerLayout*>(layout) != nullptr; };

  auto pressButton = [&editPanel](Buttons button) {
    editPanel.onButtonPressed(button, {}, true);
    editPanel.onButtonPressed(button, {}, false);
  };

  auto getFocusAndMode = [] {
    auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
    return famSetting.getState();
  };

  const auto min = std::chrono::milliseconds(20);
  const auto max = std::chrono::milliseconds(500);

  TestHelper::doMainLoop(min, max, [&] { return isParameterLayout(getLayout()); });

  SECTION("Parameter to Preset and Back")
  {
    auto oldFocus = getFocusAndMode();
    pressButton(Buttons::BUTTON_PRESET);
    TestHelper::doMainLoop(min, max, [&] { return isPresetLayout(getLayout()); });
    pressButton(Buttons::BUTTON_PRESET);
    TestHelper::doMainLoop(min, max, [&] { return oldFocus == getFocusAndMode(); });
  }

  SECTION("Parameter to Store and Back")
  {
    auto oldFocus = getFocusAndMode();
    pressButton(Buttons::BUTTON_STORE);
    TestHelper::doMainLoop(min, max, [&] { return isPresetLayout(getLayout()); });
    pressButton(Buttons::BUTTON_STORE);
    TestHelper::doMainLoop(min, max, [&] { return oldFocus == getFocusAndMode(); });
  }

  SECTION("Parameter to Store to Preset")
  {
    pressButton(Buttons::BUTTON_STORE);
    TestHelper::doMainLoop(min, max, [&] { return isPresetLayout(getLayout()); });
    pressButton(Buttons::BUTTON_PRESET);
    TestHelper::doMainLoop(min, max,
                           [&] { return isPresetLayout(getLayout()) && getFocusAndMode().mode == UIMode::Select; });
  }

  SECTION("Parameter to Preset to Store")
  {
    pressButton(Buttons::BUTTON_PRESET);
    TestHelper::doMainLoop(min, max, [&] { return isPresetLayout(getLayout()); });
    pressButton(Buttons::BUTTON_STORE);
    TestHelper::doMainLoop(min, max,
                           [&] { return isPresetLayout(getLayout()) && getFocusAndMode().mode == UIMode::Store; });
  }
}