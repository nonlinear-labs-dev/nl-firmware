#include <execinfo.h>
#include <testing/TestHelper.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"print backtrace does not exit process")
{
  Environment::printbacktrace(nltools::Log::Level::Silent);
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  ebUseCases.renamePart(VoiceGroup::I, "FooBar2001");
  auto oldName = eb->getVoiceGroupName(VoiceGroup::I);
  ebUseCases.renamePart(VoiceGroup::I, "FooBar2000");
  CHECK(oldName != eb->getVoiceGroupName(VoiceGroup::I));
}