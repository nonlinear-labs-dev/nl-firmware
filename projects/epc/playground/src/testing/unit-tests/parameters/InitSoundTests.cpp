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
  eb->undoableInitSound(transaction, Defaults::UserDefault);

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
  eb->undoableInitPart(transaction, VoiceGroup::II, Defaults::UserDefault);

  CHECK(pII->getControlPositionValue() == 0.25);
}

TEST_CASE("Special init sound value stored in layer mode is not overriden by storing in single mode")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();
  auto p = eb->findParameterByID({ C15::PID::Voice_Grp_Fade_From, VoiceGroup::I });

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);

  p->setCPFromHwui(transaction, 0.5);
  pm->storeInitSound(transaction);
  CHECK(p->getDefaultValue() == 0.5);

  p->setCPFromHwui(transaction, 1.0);
  eb->undoableConvertToSingle(transaction, VoiceGroup::I);
  pm->storeInitSound(transaction);

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
  CHECK(p->isFactoryDefaultLoaded());
  p->loadDefault(transaction, Defaults::UserDefault);
  CHECK(p->getControlPositionValue() == 0.5);
}

TEST_CASE("Special init sound value stored in layer mode is copied to part II so it is proper in split mode")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();
  auto p1 = eb->findParameterByID({ C15::PID::Unison_Detune, VoiceGroup::I });
  auto p2 = eb->findParameterByID({ C15::PID::Unison_Detune, VoiceGroup::I });

  TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);

  p1->setCPFromHwui(transaction, 0.6);
  pm->storeInitSound(transaction);

  p1->setCPFromHwui(transaction, 1.0);

  TestHelper::initDualEditBuffer<SoundType::Split>(transaction);
  p2->loadDefault(transaction, Defaults::UserDefault);
  CHECK(p2->getControlPositionValue() == 0.6);
}
