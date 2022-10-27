#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "parameters/PitchbendParameter.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "issue 3310 [3310]")
{
  PresetManagerUseCases uc(*TestHelper::getPresetManager(), *TestHelper::getSettings());
  auto bank = uc.importBankFromPath(
      std::filesystem::directory_entry { getSourceDir()
                                         + "/projects/epc/playground/test-resources/TS_Pitchprobleme.xml" },
      [](auto) {});
  auto preset = bank->getPresetAt(0);

  auto eb = TestHelper::getEditBuffer();

  auto benderPos = GENERATE(-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1);

  const auto oldMCPos = 0.5;
  const auto hwAmt = 0.5;
  const auto mcCResult = std::clamp(oldMCPos + (hwAmt * benderPos), 0.0, 1.0);

  INFO("With pre-load bender-cp: " << benderPos);

  auto bender = eb->findAndCastParameterByID<PitchbendParameter>({ C15::PID::Bender, VoiceGroup::Global });

  PhysicalControlParameterUseCases phyUseCase(bender);
  phyUseCase.changeFromAudioEngine(benderPos, HWChangeSource::TCD);

  REQUIRE(preset->findParameterByID({ C15::PID::Bender, VoiceGroup::Global }, false)->getValue() == Approx(1));
  REQUIRE(preset->findParameterByID({ C15::PID::MC_C, VoiceGroup::Global }, false)->getValue() == Approx(0.5));
  REQUIRE(preset->findParameterByID({ C15::PID::Bender_to_MC_C, VoiceGroup::Global }, false)->getValue()
          == Approx(hwAmt));

  EditBufferUseCases ebUC(*eb);
  ebUC.load(preset);

  CHECK(eb->findParameterByID({ C15::PID::Bender, VoiceGroup::Global })->getControlPositionValue()
        == Approx(benderPos));

  INFO("With new modulated MC_C-cp: " << mcCResult)

  CHECK(eb->findParameterByID({ C15::PID::MC_C, VoiceGroup::Global })->getControlPositionValue() == Approx(mcCResult));
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "When loading any preset from 'dirty' Bank leads to no changed",
                 "[3035]")
{
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());
  auto bank = pmUseCases.importBankFromPath(
      std::filesystem::directory_entry(getSourceDir() + "/projects/epc/playground/test-resources/Peter_Work_04.xml"),
      [](const auto&) {});

  TestHelper::initSingleEditBuffer();
  REQUIRE(TestHelper::getEditBuffer()->findAnyParameterChanged() == false);

  bank->forEachPreset([](const Preset* p) {
     EditBufferUseCases ebUseCases(*TestHelper::getEditBuffer());
     ebUseCases.load(p);
     TestHelper::doMainLoopIteration();

     INFO("Preset " << p->getName() << " loaded as changed");
     REQUIRE(TestHelper::getEditBuffer()->findAnyParameterChanged() == false);
  });
}