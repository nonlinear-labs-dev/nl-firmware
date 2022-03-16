#include <testing/TestHelper.h>
#include "parameters/PitchbendParameter.h"
#include "use-cases/PhysicalControlParameterUseCases.h"
#include "device-settings/GlobalLocalEnableSetting.h"
#include "use-cases/SettingsUseCases.h"
#include "parameters/PedalParameter.h"
#include "use-cases/PedalParameterUseCases.h"
#include "CompileTimeOptions.h"
#include "parameters/RibbonParameter.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

TEST_CASE("Issue 3035, Loading Preset with held bender leads to wrong values", "[3035]")
{
  auto eb = TestHelper::getEditBuffer();

  ParameterId srcID = { C15::PID::Bender, VoiceGroup::Global };
  auto bender = eb->findAndCastParameterByID<PitchbendParameter>(srcID);
  auto mcA = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto connectionParam
      = eb->findAndCastParameterByID<ModulationRoutingParameter>({ C15::PID::Bender_to_MC_A, VoiceGroup::Global });

  TestHelper::initSingleEditBuffer();

  auto settings = TestHelper::getSettings();
  auto routings = settings->getSetting<RoutingSettings>();

  routings->setAllValues(true);
  PhysicalControlParameterUseCases benderUseCase(bender);
  benderUseCase.setDefault();

  const auto modTargetId = ParameterId({ C15::PID::Shp_A_Mix, VoiceGroup::I });
  auto modTarget = eb->findParameterByID(modTargetId);

  EditBufferUseCases ebUseCase(*eb);
  ebUseCase.setParameter(modTargetId, 0);
  ebUseCase.setModulationSource(MacroControls::MC1, modTargetId);
  ebUseCase.setModulationAmount(1, modTargetId);

  MacroControlParameterUseCases mcUseCase(mcA);
  mcUseCase.setControlPosition(0.5);

  ParameterUseCases routeUseCase(connectionParam);
  routeUseCase.setControlPosition(1);

  CHECK(modTarget->getControlPositionValue() == 0);

  WHEN("Bender is -1")
  {
    benderUseCase.setControlPosition(-1.);

    THEN("Mod Checks out")
    {
      TestHelper::doMainLoopIteration();
      CHECK(bender->getControlPositionValue() == -1);
      CHECK(mcA->getControlPositionValue() == 0);
      CHECK(modTarget->getControlPositionValue() == -1);
    }
  }

  WHEN("Bender is 1")
  {
    benderUseCase.setControlPosition(1.);

    THEN("Mod Checks out")
    {
      TestHelper::doMainLoopIteration();
      CHECK(bender->getControlPositionValue() == 1);
      CHECK(mcA->getControlPositionValue() == 1);
      CHECK(modTarget->getControlPositionValue() == 1);
    }
  }

  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());

  WHEN("Bender is 1 and Held while saving")
  {
    benderUseCase.setControlPosition(1.);
    TestHelper::doMainLoopIteration();

    auto bank = pmUseCases.createBankAndStoreEditBuffer();
    auto preset = bank->getPresetAt(0);

    THEN("EditBuffer Mod Checks out")
    {
      CHECK(bender->getControlPositionValue() == 1);
      CHECK(mcA->getControlPositionValue() == 1);
      CHECK(modTarget->getControlPositionValue() == 1);
    }

    THEN("Preset Mod Checks out -> Returning Sources (and consequent Parameters appear as modulated by the default "
         "return-target postion)")
    {
      auto presetBender = preset->findParameterByID(bender->getID(), true);
      auto presetMCA = preset->findParameterByID(mcA->getID(), true);
      auto presetModTarget = preset->findParameterByID(modTarget->getID(), true);
      CHECK(presetBender->getValue() == 0);
      CHECK(presetMCA->getValue() == 0.5);
      CHECK(presetModTarget->getValue() == 0);

      THEN("HW-Position is stuck at 1")
      {
        CHECK(bender->getControlPositionValue() == 1);
        CHECK(mcA->getControlPositionValue() == 1);
        CHECK(modTarget->getControlPositionValue() == 1);
      }

      WHEN("Bender is -1")
      {
        benderUseCase.setControlPosition(-1.);

        THEN("Mod Checks out")
        {
          TestHelper::doMainLoopIteration();
          CHECK(bender->getControlPositionValue() == -1);
          CHECK(mcA->getControlPositionValue() == 0);
          CHECK(modTarget->getControlPositionValue() == -1);
        }

        WHEN("Preset is loaded")
        {
          ebUseCase.load(preset);

          THEN("Bender is -1 and Mod Checks out")
          {
            TestHelper::doMainLoopIteration();
            CHECK(bender->getControlPositionValue() == -1);
            CHECK(mcA->getControlPositionValue() == 0);
            CHECK(modTarget->getControlPositionValue() == -1);
          }
        }
      }
    }
  }
}

TEST_CASE("Load Preset with differing Return Types", "[3035]")
{
  auto pm = TestHelper::getPresetManager();
  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
  }

  auto settings = TestHelper::getSettings();
  PresetManagerUseCases uc(*pm, *settings);

  auto MC_ID = ParameterId{C15::PID::MC_A, VoiceGroup::Global};
  auto mc1 = pm->getEditBuffer()->findAndCastParameterByID<MacroControlParameter>(MC_ID);
  auto ribbon1 = pm->getEditBuffer()->findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_1, VoiceGroup::Global});
  auto pedal1 = pm->getEditBuffer()->findAndCastParameterByID<PedalParameter>({C15::PID::Pedal_1, VoiceGroup::Global});
  PedalParameterUseCases pedalUseCase(pedal1);

  auto bank = uc.importBankFromPath(std::filesystem::directory_entry{getSourceDir() + "/projects/epc/playground/test-resources/3035-5.xml"}, [](auto){});
  auto init = bank->getPresetAt(0);
  auto ped1_0 = bank->getPresetAt(1);
  auto PB_AT = bank->getPresetAt(2);
  auto PB_AT_2 = bank->getPresetAt(3);
  auto ped1_nonret = bank->getPresetAt(4);
  auto ped1_retzero = bank->getPresetAt(5);
  auto ped1_retcenter = bank->getPresetAt(6);
  auto ped4_nonret = bank->getPresetAt(7);
  auto ped4_retzero = bank->getPresetAt(8);
  auto ped4_retzero2 = bank->getPresetAt(9);
  auto rib1_stay = bank->getPresetAt(10);
  auto rib1_retcenter = bank->getPresetAt(12);
  auto rib1_retcenter_2 = bank->getPresetAt(13);

  EditBufferUseCases ebUseCases(*pm->getEditBuffer());

  WHEN("Pedal Load Init -> Move -> Load Ped 1")
  {
    ebUseCases.load(init);
    TestHelper::doMainLoopIteration();
    pedalUseCase.changeFromAudioEngine(1, HWChangeSource::TCD);
    ebUseCases.load(ped1_0);
    TestHelper::doMainLoopIteration();
    CHECK(pedal1->getDisplayString() == "100.0 %");
  }

  WHEN("Pedal Load Non Ret -> Move -> Ret Zero")
  {
    ebUseCases.load(ped1_nonret);
    TestHelper::doMainLoopIteration();
    pedalUseCase.changeFromAudioEngine(0.25, HWChangeSource::TCD);
    ebUseCases.load(ped1_retzero);
    TestHelper::doMainLoopIteration();
    CHECK(pedal1->getDisplayString() == "25.0 %");
  }

  WHEN("Pedal Load Return to Zero after Stay")
  {
    ebUseCases.load(ped1_nonret);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_nonret->findParameterByID(MC_ID, true)->getValue());
    ebUseCases.load(ped1_retzero);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_retzero->findParameterByID(MC_ID, true)->getValue());
  }

  WHEN("Pedal Load Return to Center after Stay")
  {
    ebUseCases.load(ped1_nonret);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_nonret->findParameterByID(MC_ID, true)->getValue());
    ebUseCases.load(ped1_retcenter);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_retcenter->findParameterByID(MC_ID, true)->getValue());
  }

  WHEN("Pedal Load Stay after Return to Zero")
  {
    ebUseCases.load(ped1_retzero);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_retzero->findParameterByID(MC_ID, true)->getValue());
    ebUseCases.load(ped1_nonret);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_nonret->findParameterByID(MC_ID, true)->getValue());
  }

  WHEN("Pedal Load Stay after Return to Center")
  {
    ebUseCases.load(ped1_retcenter);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_retcenter->findParameterByID(MC_ID, true)->getValue());
    ebUseCases.load(ped1_nonret);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == ped1_nonret->findParameterByID(MC_ID, true)->getValue());
  }

  WHEN("Ribbon Load Stay after Return to Center")
  {
    ebUseCases.load(rib1_retcenter);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == rib1_retcenter->findParameterByID(MC_ID, true)->getValue());
    CHECK(Approx(ribbon1->getControlPositionValue()) == 0);
    ebUseCases.load(rib1_stay);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == rib1_stay->findParameterByID(MC_ID, true)->getValue());
    CHECK(Approx(ribbon1->getControlPositionValue()) == rib1_stay->findParameterByID(MC_ID, true)->getValue());
  }

  WHEN("Ribbon Load Return to Center after Stay")
  {
    ebUseCases.load(rib1_stay);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == rib1_stay->findParameterByID(MC_ID, true)->getValue());
    ebUseCases.load(rib1_retcenter);
    TestHelper::doMainLoopIteration();
    CHECK(Approx(mc1->getControlPositionValue()) == rib1_retcenter->findParameterByID(MC_ID, true)->getValue());
  }
}