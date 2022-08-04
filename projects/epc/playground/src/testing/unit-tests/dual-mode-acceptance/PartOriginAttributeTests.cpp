#include <testing/TestHelper.h>
#include <Application.h>
#include <presets/Preset.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <use-cases/DirectLoadUseCases.h>
#include "use-cases/SettingsUseCases.h"

namespace detail
{
  auto getDirectLoad()
  {
    return Application::get().getSettings()->getSetting<DirectLoadSetting>();
  }

  auto pressButton(Buttons b)
  {
    auto hwui = Application::get().getHWUI();
    hwui->getPanelUnit().getEditPanel().onButtonPressed(b, {}, true);
    hwui->getPanelUnit().getEditPanel().onButtonPressed(b, {}, false);
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Part Origin Attribute")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  auto hwui = Application::get().getHWUI();

  MockPresetStorage presets;

  DirectLoadUseCases useCase(detail::getDirectLoad());
  useCase.disableDirectLoad();
  SettingsUseCases useCases(*TestHelper::getSettings());
  useCases.setFocusAndMode(UIDetail::Init);

  SECTION("Load Single Full")
  {
    ebUseCases.load(presets.getSinglePreset());

    auto originI = eb->getPartOrigin(VoiceGroup::I);
    auto originII = eb->getPartOrigin(VoiceGroup::II);

    CHECK(originI.presetUUID == presets.getSinglePreset()->getUuid());
    CHECK(originI.sourceGroup == VoiceGroup::I);
    CHECK(originII.presetUUID == presets.getSinglePreset()->getUuid());
    CHECK(originII.sourceGroup == VoiceGroup::I);
  }

  SECTION("Init Sound Resets")
  {
    ebUseCases.load(presets.getSplitPreset());
    ebUseCases.initSound(Defaults::FactoryDefault);

    auto originI = eb->getPartOrigin(VoiceGroup::I);
    auto originII = eb->getPartOrigin(VoiceGroup::II);

    CHECK(originI.presetUUID == Uuid::none());
    CHECK(originII.presetUUID == Uuid::none());
    CHECK(originI.sourceGroup == VoiceGroup::Global);
    CHECK(originII.sourceGroup == VoiceGroup::Global);
  }

  SECTION("Init Part Resets")
  {
    ebUseCases.load(presets.getSinglePreset());
    ebUseCases.initPart(VoiceGroup::I, Defaults::FactoryDefault);

    auto originI = eb->getPartOrigin(VoiceGroup::I);
    CHECK(originI.presetUUID == Uuid::none());
    CHECK(originI.sourceGroup == VoiceGroup::Global);

    ebUseCases.initPart(VoiceGroup::II, Defaults::FactoryDefault);
    auto originII = eb->getPartOrigin(VoiceGroup::II);
    CHECK(originII.presetUUID == Uuid::none());
    CHECK(originII.sourceGroup == VoiceGroup::Global);
  }

  SECTION("Load Dual Full")
  {
    ebUseCases.load(presets.getLayerPreset());
    auto originI = eb->getPartOrigin(VoiceGroup::I);
    auto originII = eb->getPartOrigin(VoiceGroup::II);

    CHECK(originI.presetUUID == presets.getLayerPreset()->getUuid());
    CHECK(originII.presetUUID == presets.getLayerPreset()->getUuid());
    CHECK(originI.sourceGroup == VoiceGroup::I);
    CHECK(originII.sourceGroup == VoiceGroup::II);
  }

  SECTION("Load Part Of Dual")
  {
    TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
    auto scope = TestHelper::createTestScope();

    eb->undoableLoadToPart(scope->getTransaction(), presets.getSplitPreset(), VoiceGroup::I, VoiceGroup::I);

    auto originI = eb->getPartOrigin(VoiceGroup::I);
    CHECK(originI.presetUUID == presets.getSplitPreset()->getUuid());
    CHECK(originI.sourceGroup == VoiceGroup::I);

    eb->undoableLoadToPart(scope->getTransaction(), presets.getSplitPreset(), VoiceGroup::II, VoiceGroup::II);

    auto originII = eb->getPartOrigin(VoiceGroup::II);
    CHECK(originII.presetUUID == presets.getSplitPreset()->getUuid());
    CHECK(originII.sourceGroup == VoiceGroup::II);
  }
}

auto getOrigins()
{
  auto eb = TestHelper::getEditBuffer();
  return std::pair { eb->getPartOrigin(VoiceGroup::I), eb->getPartOrigin(VoiceGroup::II) };
};

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Step Direct Load and Load to Part Preset List", "[Preset][Loading]")
{
  DualPresetBank presets;
  auto bank = presets.getBank();
  auto hwui = Application::get().getHWUI();

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

  auto eb = TestHelper::getEditBuffer();
  auto settings = TestHelper::getSettings();
  auto pm = Application::get().getPresetManager();
  auto vgManager = Application::get().getVGManager();
  vgManager->setCurrentVoiceGroup(VoiceGroup::I);
  auto currentVG = vgManager->getCurrentVoiceGroup();

  CHECK(currentVG == VoiceGroup::I);

  auto [ogI, ogII] = getOrigins();

  CHECK(eb->getOrigin() == nullptr);
  CHECK(ogI.presetUUID == Uuid::none());
  CHECK(ogII.presetUUID == Uuid::none());
  CHECK(eb->getUUIDOfLastLoadedPreset() == Uuid::init());

  auto min = std::chrono::milliseconds(20);
  auto max = std::chrono::milliseconds(500);

  SECTION("Select First Preset in Bank")
  {

    DirectLoadUseCases dlUseCase(detail::getDirectLoad());
    dlUseCase.enableDirectLoadFromWebUI(nullptr, VoiceGroup::Global, VoiceGroup::Global);

    {
      PresetManagerUseCases useCase(*pm, *settings);
      useCase.selectPreset(bank->getPresetAt(0));
      SettingsUseCases useCases(*TestHelper::getSettings());
      useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
    }

    TestHelper::doMainLoop(min, max, [&]() {
      auto uuid = eb->getUUIDOfLastLoadedPreset();
      return uuid == bank->getPresetAt(0)->getUuid();
    });

    vgManager->setLoadToPart(true);

    detail::pressButton(Buttons::BUTTON_INC);

    TestHelper::doMainLoop(min, max, [&]() {
      auto [ogI, ogII] = getOrigins();
      auto x = ogI.presetUUID == bank->getPresetAt(0)->getUuid();
      return x && ogI.sourceGroup == VoiceGroup::II;
    });

    detail::pressButton(Buttons::BUTTON_INC);
    TestHelper::doMainLoop(min, max, [&] {
      auto [ogI, ogII] = getOrigins();
      auto x = ogI.presetUUID == bank->getPresetAt(1)->getUuid();
      return x && ogI.sourceGroup == VoiceGroup::I;
    });

    detail::pressButton(Buttons::BUTTON_INC);
    TestHelper::doMainLoop(min, max, [&] {
      auto [ogI, ogII] = getOrigins();
      auto x = ogI.presetUUID == bank->getPresetAt(1)->getUuid();
      return x && ogI.sourceGroup == VoiceGroup::II;
    });

    detail::pressButton(Buttons::BUTTON_DEC);
    TestHelper::doMainLoop(min, max, [&] {
      auto [ogI, ogII] = getOrigins();
      auto x = ogI.presetUUID == bank->getPresetAt(1)->getUuid();
      return x && ogI.sourceGroup == VoiceGroup::I;
    });

    detail::pressButton(Buttons::BUTTON_DEC);
    TestHelper::doMainLoop(min, max, [&] {
      auto [ogI, ogII] = getOrigins();
      auto x = ogI.presetUUID == bank->getPresetAt(0)->getUuid();
      return x && ogI.sourceGroup == VoiceGroup::II;
    });

    detail::pressButton(Buttons::BUTTON_DEC);
    TestHelper::doMainLoop(min, max, [&] {
      auto [ogI, ogII] = getOrigins();
      auto x = ogI.presetUUID == bank->getPresetAt(0)->getUuid();
      return x && ogI.sourceGroup == VoiceGroup::I;
    });
  }
}
