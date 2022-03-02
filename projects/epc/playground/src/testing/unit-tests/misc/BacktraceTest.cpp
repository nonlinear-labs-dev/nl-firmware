#include <execinfo.h>
#include <testing/TestHelper.h>

TEST_CASE("Backtrace does not exit process")
{
  nltools::Log::printbacktrace(nltools::Log::Level::Error);

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  ebUseCases.renamePart(VoiceGroup::I, "FooBar2001");
  auto oldName = eb->getVoiceGroupName(VoiceGroup::I);
  ebUseCases.renamePart(VoiceGroup::I, "FooBar2000");
  CHECK(oldName != eb->getVoiceGroupName(VoiceGroup::I));
}