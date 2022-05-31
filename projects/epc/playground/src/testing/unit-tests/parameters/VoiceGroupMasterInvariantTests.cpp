#include <testing/TestHelper.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Default value for Fade From is different in I and II")
{
  auto eb = TestHelper::getEditBuffer();
  CHECK(TestHelper::floating::differs(
      eb->findParameterByID({ 396, VoiceGroup::I })->getValue().getFactoryDefaultValue(),
      eb->findParameterByID({ 396, VoiceGroup::II })->getValue().getFactoryDefaultValue()));
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Fade From gets properly initialized on convert to Layer")
{
  TestHelper::initSingleEditBuffer();

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  useCase.convertToLayer(VoiceGroup::I);

  CHECK(eb->findParameterByID({ 396, VoiceGroup::I })->getDisplayString() == "C6");
  CHECK(eb->findParameterByID({ 396, VoiceGroup::II })->getDisplayString() == "C1");
}
