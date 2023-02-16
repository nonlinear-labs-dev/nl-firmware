#include <testing/TestHelper.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Split to Single With Split-Point Selected")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  WHEN("is Split")
  {
    TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);
    ebUseCases.selectParameter({ C15::PID::Split_Split_Point, VoiceGroup::I }, false);
    CHECK(eb->getSelectedParameterNumber() == C15::PID::Split_Split_Point);
    CHECK(Application::get().getVGManager()->getCurrentVoiceGroup() == VoiceGroup::I);

    WHEN("Converted to Single!")
    {
      ebUseCases.convertToSingle(VoiceGroup::I);
      CHECK(eb->getSelectedParameterNumber() == C15::PID::Master_Volume);
      CHECK(Application::get().getVGManager()->getCurrentVoiceGroup() == VoiceGroup::I);
    }
  }
}