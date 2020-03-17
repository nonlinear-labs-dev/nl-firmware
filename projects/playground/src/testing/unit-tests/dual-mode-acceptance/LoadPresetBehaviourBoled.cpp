#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <device-settings/LoadToPartSetting.h>
#include <device-settings/DirectLoadSetting.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include <presets/Preset.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/SelectVoiceGroupLayout.h>

namespace detail
{
  auto getDirectLoad()
  {
    return Application::get().getSettings()->getSetting<DirectLoadSetting>();
  }

  auto getLoadToPart()
  {
    return Application::get().getSettings()->getSetting<LoadToPartSetting>();
  }

  auto getHWUI()
  {
    return Application::get().getHWUI();
  }

  auto pressButton(Buttons b)
  {
    getHWUI()->getPanelUnit().getEditPanel().onButtonPressed(b, {}, true);
    getHWUI()->getPanelUnit().getEditPanel().onButtonPressed(b, {}, false);
  }

  auto getLayout()
  {
    return getHWUI()->getPanelUnit().getEditPanel().getBoled().getLayout();
  }

  auto createBank()
  {
    auto scope = TestHelper::createTestScope();
    auto pm = TestHelper::getPresetManager();
    auto bank = pm->addBank(scope->getTransaction());
    return bank;
  }

  template <SoundType tType> auto addPresetToBank(Bank* bank)
  {
    auto scope = TestHelper::createTestScope();
    auto preset = bank->appendPreset(scope->getTransaction());
    preset->setType(scope->getTransaction(), tType);
    bank->ensurePresetSelection(scope->getTransaction());
    return preset;
  }

  void selectBank(Bank* b)
  {
    auto scope = TestHelper::createTestScope();
    TestHelper::getPresetManager()->selectBank(scope->getTransaction(), b->getUuid());
  }
}

TEST_CASE("StepPresetList")
{
  auto assertIsPMLayout = [] { return dynamic_cast<const PresetManagerLayout*>(detail::getLayout().get()) != nullptr; };
  auto assertInPartSelectOverlay
      = [] { return dynamic_cast<const SelectVoiceGroupLayout*>(detail::getLayout().get()) != nullptr; };

  auto eb = TestHelper::getEditBuffer();
  auto pm = TestHelper::getPresetManager();

  {
    detail::getLoadToPart()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);
    detail::getDirectLoad()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);

    {
      auto scope = TestHelper::createTestScope();
      TestHelper::initSingleEditBuffer(scope->getTransaction());

      detail::getHWUI()->undoableSetFocusAndMode(scope->getTransaction(),
                                                 { UIFocus::Presets, UIMode::Select, UIDetail::Init });
      CHECK(assertIsPMLayout());
    }
  }

  SECTION("Bank with Single Presets")
  {
    auto bank = detail::createBank();
    detail::addPresetToBank<SoundType::Single>(bank);
    detail::addPresetToBank<SoundType::Single>(bank);
    detail::selectBank(bank);

    const auto loadedPresetUUID = eb->getUUIDOfLastLoadedPreset();
    CHECK(loadedPresetUUID == Uuid::init());
    CHECK(eb->getType() == SoundType::Single);

    SECTION("w/o direct Load")
    {
      detail::getDirectLoad()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);

      CHECK(assertIsPMLayout());
      detail::pressButton(Buttons::BUTTON_INC);
      CHECK(eb->getUUIDOfLastLoadedPreset() == loadedPresetUUID);
      CHECK(bank->getSelectedPreset() == bank->getPresetAt(1));
    }

    SECTION("w/ direct load")
    {
      detail::getDirectLoad()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);

      CHECK(assertIsPMLayout());
      detail::pressButton(Buttons::BUTTON_INC);

      if(pm->isAutoLoadScheduled())
        pm->forceScheduledAutoLoad();

      CHECK(bank->getSelectedPreset() == bank->getPresetAt(1));
      CHECK(eb->getUUIDOfLastLoadedPreset() == bank->getPresetAt(1)->getUuid());
    }
  }

  SECTION("Bank with Dual Presets")
  {
    auto bank = detail::createBank();
    detail::addPresetToBank<SoundType::Layer>(bank);
    detail::addPresetToBank<SoundType::Layer>(bank);
    detail::selectBank(bank);

    const auto loadedPresetUUID = eb->getUUIDOfLastLoadedPreset();
    CHECK(loadedPresetUUID == Uuid::init());
    CHECK(eb->getType() == SoundType::Single);

    SECTION("w/o direct Load")
    {
      detail::getDirectLoad()->set(BooleanSettings::BOOLEAN_SETTING_FALSE);

      CHECK(assertIsPMLayout());
      detail::pressButton(Buttons::BUTTON_INC);

      CHECK(eb->getType() == SoundType::Single);
      CHECK(eb->getUUIDOfLastLoadedPreset() == loadedPresetUUID);
      CHECK(bank->getSelectedPreset() == bank->getPresetAt(1));
    }

    SECTION("w/ direct load")
    {
      detail::getDirectLoad()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);

      CHECK(assertIsPMLayout());
      detail::pressButton(Buttons::BUTTON_INC);

      if(pm->isAutoLoadScheduled())
        pm->forceScheduledAutoLoad();

      CHECK(eb->getType() == SoundType::Layer);
      CHECK(bank->getSelectedPreset() == bank->getPresetAt(1));
      CHECK(eb->getUUIDOfLastLoadedPreset() == bank->getPresetAt(1)->getUuid());
    }

    SECTION("w/ load to part and direct load in dual sound")
    {
      TestHelper::initDualEditBuffer<SoundType::Split>();
      detail::getDirectLoad()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);
      detail::getLoadToPart()->set(BooleanSettings::BOOLEAN_SETTING_TRUE);

      CHECK(eb->getType() == SoundType::Split);

      CHECK(assertIsPMLayout());

      detail::pressButton(Buttons::BUTTON_INC);

      CHECK(assertInPartSelectOverlay());
    }
  }
}