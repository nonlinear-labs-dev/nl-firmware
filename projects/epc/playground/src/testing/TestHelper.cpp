#include "TestHelper.h"
#include "testing/unit-tests/mock/EditBufferNamedLogicalParts.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/types.h>

void TestHelper::randomizeCrossFBAndToFX(UNDO::Transaction* transaction)
{
  using EBL = EditBufferLogicalParts;

  for(auto p : EBL::getCrossFB<VoiceGroup::I>())
  {
    p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
  }

  for(auto p : EBL::getToFX<VoiceGroup::I>())
  {
    p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
  }

  for(auto p : EBL::getCrossFB<VoiceGroup::II>())
  {
    p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
  }

  for(auto p : EBL::getToFX<VoiceGroup::II>())
  {
    p->undoableRandomize(transaction, Initiator::EXPLICIT_OTHER, 1);
  }
};

void TestHelper::randomizeFadeParams(UNDO::Transaction* transaction)
{
  using EBL = EditBufferLogicalParts;

  TestHelper::forceParameterChange(transaction, EBL::getFadeFrom<VoiceGroup::I>());
  TestHelper::forceParameterChange(transaction, EBL::getFadeFrom<VoiceGroup::II>());
  TestHelper::forceParameterChange(transaction, EBL::getFadeRange<VoiceGroup::I>());
  TestHelper::forceParameterChange(transaction, EBL::getFadeRange<VoiceGroup::II>());
};

int TestHelper::getNumberOfFDs()
{
  DIR* dp = opendir("/proc/self/fd");
  struct dirent* de;
  int count = -3;  // '.', '..', dp

  if(dp == NULL)
    return -1;

  while((de = readdir(dp)) != NULL)
    count++;

  (void) closedir(dp);

  return count;
}

TestHelper::ApplicationFixture::ApplicationFixture()
{
  nltools::Log::setLevel(nltools::Log::Error);
  Glib::MainContext::get_default();
  m_numFileDescriptors = getNumberOfFDs();
  app = std::make_unique<Application>(0, nullptr);
  nltools::Log::setLevel(nltools::Log::Error);
}

TestHelper::ApplicationFixture::~ApplicationFixture()
{
  app->quit();
  app.reset(nullptr);
  auto n = getNumberOfFDs();

  if(m_numFileDescriptors != n)
  {
    nltools::Log::info("Test leaks filedescriptors, before:", m_numFileDescriptors, ", now:", n);
  }
}
