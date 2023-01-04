#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include <presets/Preset.h>
#include <presets/Bank.h>
#include <presets/PresetParameter.h>
#include <parameters/scale-converters/ScaleConverter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "compare issue")
{
  // import bank
  PresetManagerUseCases pmUseCases(*app->getPresetManager(), *app->getSettings());
  auto bank = pmUseCases.importBankFromPath(
      std::filesystem::directory_entry { getSourceDir() + "/projects/epc/playground/test-resources/CompareIssue.xml" },
      [](auto) {});
  auto preset = bank->getPresetAt(0);

  EditBufferUseCases ebUseCases(*app->getPresetManager()->getEditBuffer());
  ebUseCases.load(preset);

  //do compare
  auto combDecay
      = app->getPresetManager()->getEditBuffer()->findParameterByID({ C15::PID::Comb_Flt_Decay, VoiceGroup::I });
  auto parameterCP = preset->findParameterByID({ C15::PID::Comb_Flt_Decay, VoiceGroup::I }, false)->getValue();
  auto cp = combDecay->getControlPositionValue();

  CHECK(combDecay->getValue().getQuantizedValue(parameterCP, true)
        == combDecay->getValue().getQuantizedValue(cp, true));
}