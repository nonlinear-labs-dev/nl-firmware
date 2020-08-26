#include <testing/TestHelper.h>
#include <parameter_list.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <catch.hpp>

TEST_CASE("Init sound stored as single but used in dual")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);

  auto pI = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::I });
  auto pII = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::II });

  pI->setCPFromHwui(transaction, 0.25);
  pII->setCPFromHwui(transaction, 0.75);

  eb->undoableConvertToSingle(transaction, VoiceGroup::I);
  pm->storeInitSound(transaction);

  pI->setCPFromHwui(transaction, 1);
  pII->setCPFromHwui(transaction, 1);

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
  eb->undoableInitSound(transaction);

  CHECK(pI->getControlPositionValue() == 0.25);
  CHECK(pII->getControlPositionValue() == 0.25);
}

TEST_CASE("Init sound stored as single but used in part II")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);

  auto pI = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::I });
  auto pII = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::II });

  pI->setCPFromHwui(transaction, 0.25);
  pII->setCPFromHwui(transaction, 0.75);

  eb->undoableConvertToSingle(transaction, VoiceGroup::I);
  pm->storeInitSound(transaction);

  pII->setCPFromHwui(transaction, 1);

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
  eb->undoableInitPart(transaction, VoiceGroup::II);

  CHECK(pII->getControlPositionValue() == 0.25);
}
