#include <testing/TestHelper.h>
#include <parameter_list.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <catch.hpp>

TEST_CASE("Init sound stored as single and used in single")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  SoundUseCases soundUseCases(eb, pm);

  TestHelper::initSingleEditBuffer();

  auto pI = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::I });
  auto pII = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::II });

  ParameterUseCases p1UseCase(pI);

  p1UseCase.setControlPosition(0.25);
  useCase.convertToSingle(VoiceGroup::I);
  soundUseCases.storeInitSound();
  p1UseCase.setControlPosition(1);

  TestHelper::initSingleEditBuffer();
  useCase.initSound(Defaults::UserDefault);

  CHECK(pI->getControlPositionValue() == 0.25);
}

TEST_CASE("Init sound stored as single but used in dual")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  SoundUseCases soundUseCases(eb, pm);

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

  auto pI = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::I });
  auto pII = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::II });

  ParameterUseCases p1UseCase(pI);
  ParameterUseCases p2UseCase(pII);

  p1UseCase.setControlPosition(0.25);
  p2UseCase.setControlPosition(0.75);
  useCase.convertToSingle(VoiceGroup::I);
  soundUseCases.storeInitSound();
  p1UseCase.setControlPosition(1);
  p2UseCase.setControlPosition(1);

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
  useCase.initSound(Defaults::UserDefault);

  CHECK(pI->getControlPositionValue() == 0.25);
  CHECK(pII->getControlPositionValue() == 0.25);
}

TEST_CASE("Init sound stored as single but used in part II")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  SoundUseCases soundUseCases(eb, pm);

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

  auto pI = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::I });
  auto pII = eb->findParameterByID({ C15::PID::ParameterID::Env_A_Att, VoiceGroup::II });

  ParameterUseCases p1UseCase(pI);
  ParameterUseCases p2UseCase(pII);

  p1UseCase.setControlPosition(0.25);
  p2UseCase.setControlPosition(0.75);
  useCase.convertToSingle(VoiceGroup::I);
  soundUseCases.storeInitSound();
  p2UseCase.setControlPosition(1);

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
  useCase.initPart(VoiceGroup::II, Defaults::UserDefault);

  CHECK(pII->getControlPositionValue() == 0.25);
}

TEST_CASE("Special init sound value stored in layer mode is not overriden by storing in single mode")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  SoundUseCases soundUseCases(eb, pm);
  auto p = eb->findParameterByID({ C15::PID::Voice_Grp_Fade_From, VoiceGroup::I });

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

  ParameterUseCases pUseCase(p);

  pUseCase.setControlPosition(0.5);
  soundUseCases.storeInitSound();
  CHECK(p->getDefaultValue() == 0.5);

  pUseCase.setControlPosition(1.0);
  useCase.convertToSingle(VoiceGroup::I);
  soundUseCases.storeInitSound();

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);
  CHECK(p->isFactoryDefaultLoaded());

  pUseCase.loadDefault(Defaults::UserDefault);
  CHECK(p->getControlPositionValue() == 0.5);
}

TEST_CASE("Special init sound value stored in layer mode is copied to part II so it is proper in split mode")
{
  auto pm = TestHelper::getPresetManager();
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebU(*eb);
  SoundUseCases soundU(eb, pm);

  auto p1 = eb->findParameterByID({ C15::PID::Unison_Detune, VoiceGroup::I });
  auto p2 = eb->findParameterByID({ C15::PID::Unison_Detune, VoiceGroup::I });

  TestHelper::initDualEditBuffer<SoundType::Layer>(VoiceGroup::I);

  ParameterUseCases p1U(p1);
  ParameterUseCases p2U(p2);

  p1U.setControlPosition(0.6);
  soundU.storeInitSound();
  p1U.setControlPosition(1.0);

  TestHelper::initDualEditBuffer<SoundType::Split>(VoiceGroup::I);

  p2U.loadDefault(Defaults::UserDefault);
  CHECK(p2->getControlPositionValue() == 0.6);
}
