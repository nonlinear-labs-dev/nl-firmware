#include "TestHelper.h"
#include "testing/unit-tests/mock/EditBufferNamedLogicalParts.h"
#include <sys/time.h>
#include <sys/resource.h>

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

int getNumberOfFDs(int pid)
{
  auto command = StringTools::buildString("ls -la /proc/", pid, "/fd");
  SpawnCommandLine cmd(command);
  auto cout = cmd.getStdOutput();
  auto numFDs = std::count(cout.begin(), cout.end(), '\n');
  return numFDs;
}

TestHelper::ApplicationFixture::ApplicationFixture()
{
  auto pid = getpid();
  nltools::Log::error("before con", getNumberOfFDs(pid));
  app = std::make_unique<Application>(0, nullptr);
  nltools::Log::error("after con", getNumberOfFDs(pid));
}

TestHelper::ApplicationFixture::~ApplicationFixture()
{
  auto pid = getpid();
  nltools::Log::error("before destr", getNumberOfFDs(pid));
  app.reset(nullptr);
  nltools::Log::error("after destr", getNumberOfFDs(pid));
}
