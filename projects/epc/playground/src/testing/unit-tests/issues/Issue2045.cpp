#include <testing/TestHelper.h>
#include <proxies/hwui/HWUI.h>
#include "proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupIndicator.h"
#include "use-cases/VoiceGroupUseCases.h"
#include <http/UndoScope.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "HWUI - Part Selection not visible after UNDO")
{
  using namespace std::chrono_literals;

  auto hwui = app->getHWUI();
  auto vgManager = app->getVGManager();

  auto eb = app->getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  auto parameterToSelect = GENERATE(C15::PID::Env_A_Att_Vel, C15::PID::FB_Mix_Asym, C15::PID::Osc_A_Chirp);

  TestHelper::initDualEditBuffer<SoundType::Split>(vgManager->getCurrentVoiceGroup());

  INFO("Selecting Parameter " << parameterToSelect);
  ebUseCases.selectParameter({parameterToSelect, VoiceGroup::I});

  auto isInParameterLayout = [hwui](){
    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
    return dynamic_cast<ParameterLayout2*>(layout) != nullptr;
  };

  auto getVoiceGroupIndicator = [hwui](){
    auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get();
    return layout->findControlOfType<VoiceGroupIndicator>();
  };

  WHEN("WHEN part I is selected")
  {
    REQUIRE(vgManager->getCurrentVoiceGroup() == VoiceGroup::I);
    REQUIRE(isInParameterLayout());
    REQUIRE(getVoiceGroupIndicator() != nullptr);
    REQUIRE(getVoiceGroupIndicator()->getCurrentDisplayedVoiceGroup() == VoiceGroup::I);

    THEN("Switch VoiceGroup")
    {
      VoiceGroupUseCases vgUseCases(vgManager, eb);
      vgUseCases.toggleVoiceGroupSelection();

      TestHelper::doMainLoopFor(10ms);

      REQUIRE(vgManager->getCurrentVoiceGroup() == VoiceGroup::II);
      REQUIRE(isInParameterLayout());
      REQUIRE(getVoiceGroupIndicator() != nullptr);
      REQUIRE(getVoiceGroupIndicator()->getCurrentDisplayedVoiceGroup() == VoiceGroup::II);

      WHEN("Undo Pressed!")
      {
        app->getUndoScope()->undo();

        TestHelper::doMainLoopFor(10ms);

        REQUIRE(vgManager->getCurrentVoiceGroup() == VoiceGroup::I);
        REQUIRE(isInParameterLayout());
        REQUIRE(getVoiceGroupIndicator() != nullptr);
        REQUIRE(getVoiceGroupIndicator()->getCurrentDisplayedVoiceGroup() == VoiceGroup::I);
      }
    }
  }
}