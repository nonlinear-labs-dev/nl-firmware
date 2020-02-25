#include <testing/TestHelper.h>

TEST_CASE("Convert Layer to Split Sound", "[EditBuffer][Convert]")
{
  auto eb = TestHelper::getEditBuffer();
  auto splitPoint = eb->findParameterByID({ 356, VoiceGroup::Global });

  REQUIRE(splitPoint != nullptr);
  TestHelper::initDualEditBuffer<SoundType::Layer>();

  //Prepare changed splitPoint
  {
    auto scope = TestHelper::createTestScope();
    splitPoint->setCPFromHwui(scope->getTransaction(), 0.0);
  }

  //Convert to Split
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableConvertToDual(scope->getTransaction(), SoundType::Split);
  }

  CHECK_PARAMETER_CP_EQUALS_FICTION(splitPoint, 0.5);
}