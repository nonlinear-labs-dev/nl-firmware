#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "issue 3310")
{
  PresetManagerUseCases uc(*TestHelper::getPresetManager(), *TestHelper::getSettings());
  auto bank = uc.importBankFromPath(std::filesystem::directory_entry{getSourceDir() + "/projects/epc/playground/test-resources/TS_Pitchprobleme.xml"}, [](auto){});
  auto preset = bank->getPresetAt(0);

  auto eb = TestHelper::getEditBuffer();
  auto oldBenderPos = eb->findParameterByID({C15::PID::Bender, VoiceGroup::Global})->getControlPositionValue();

  CHECK(preset->findParameterByID({C15::PID::Bender, VoiceGroup::Global}, false)->getValue() == Approx(1));
  CHECK(preset->findParameterByID({C15::PID::MC_C, VoiceGroup::Global}, false)->getValue() == Approx(0.5));

  EditBufferUseCases ebUC(*eb);
  ebUC.load(preset);
  CHECK(eb->findParameterByID({C15::PID::Bender, VoiceGroup::Global})->getControlPositionValue() == Approx(oldBenderPos));
  CHECK(eb->findParameterByID({C15::PID::MC_C, VoiceGroup::Global})->getControlPositionValue() == Approx(0.5));
}