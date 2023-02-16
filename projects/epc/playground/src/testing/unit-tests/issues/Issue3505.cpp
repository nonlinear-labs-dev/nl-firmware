#include <testing/TestHelper.h>

namespace
{
  VoiceGroup invert(VoiceGroup vg)
  {
    return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "convert dual to single keeps FX II")
{
  auto& eb = *app->getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(eb);

  auto ogSoundType = GENERATE(SoundType::Layer, SoundType::Split);
  auto srcGroup = GENERATE(VoiceGroup::I, VoiceGroup::II);

  ebUseCases.initSoundAs(ogSoundType, Defaults::FactoryDefault);

  WHEN(toString(ogSoundType) << "-" << toString(srcGroup) << " is converted to single")
  {
    auto other = invert(srcGroup);

    auto testParameterMy = eb.findParameterByID({ C15::PID::Reverb_Mix, srcGroup });
    auto testParameterOther = eb.findParameterByID({ C15::PID::Reverb_Mix, other });
    ParameterUseCases ucMy(testParameterMy);
    ParameterUseCases ucOther(testParameterOther);

    ucMy.setControlPosition(0.787);
    ucOther.setControlPosition(0.187);

    ebUseCases.convertToSingle(srcGroup);

    CHECK(testParameterMy->getControlPositionValue() == Approx(0.787));
    CHECK(testParameterOther->getControlPositionValue() == Approx(0.187));
  }
}