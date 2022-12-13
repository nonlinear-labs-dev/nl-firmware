#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include "parameters/RibbonParameter.h"
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>

namespace {
  auto getSoftwareVersionOfPreset(Bank* b, int num)
  {
    return b->getPresetAt(num)->getAttribute("SoftwareVersion", "");
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "load ribbons correctly")
{

  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());
  auto bank = pmUseCases.importBankFromPath(
      std::filesystem::directory_entry { to_string(getSourceDir())
                                         + std::string("/projects/epc/playground/test-resources/Ribbon_Bug.xml") },
      [](auto s) {});

  REQUIRE(bank->getNumPresets() == 6);
  REQUIRE(getSoftwareVersionOfPreset(bank, 0) == "rc-22-48");
  REQUIRE(getSoftwareVersionOfPreset(bank, 1) == "rc-22-48");
  REQUIRE(getSoftwareVersionOfPreset(bank, 2) == "22-44"); // but is rather rc-22-48
  REQUIRE(getSoftwareVersionOfPreset(bank, 3) == "22-44"); // same as above
  REQUIRE(getSoftwareVersionOfPreset(bank, 4) == "22-44");
  REQUIRE(getSoftwareVersionOfPreset(bank, 5) == "22-44");

  WHEN("Preset 0 is Loaded")
  {
    auto& eb = *TestHelper::getEditBuffer();
    auto ribbon1 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_1, VoiceGroup::Global});
    auto ribbon2 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_2, VoiceGroup::Global});
    auto ribbon3 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_3, VoiceGroup::Global});
    auto ribbon4 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_4, VoiceGroup::Global});
    auto mcA = eb.findAndCastParameterByID<MacroControlParameter>({C15::PID::MC_A, VoiceGroup::Global});
    auto mcB = eb.findAndCastParameterByID<MacroControlParameter>({C15::PID::MC_B, VoiceGroup::Global});
    auto mcC = eb.findAndCastParameterByID<MacroControlParameter>({C15::PID::MC_C, VoiceGroup::Global});
    auto mcD = eb.findAndCastParameterByID<MacroControlParameter>({C15::PID::MC_D, VoiceGroup::Global});

    auto ribbon1ToA = eb.findParameterByID({C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global});

    EditBufferUseCases ebUseCases(eb);

    auto presetNum = GENERATE(0, 1);

    for(auto i = 0; i < 10; i++)
    {
      INFO("preset " << presetNum << "  load number " << i << " not loaded correctly");
      auto preset = bank->getPresetAt(presetNum);
      ebUseCases.load(preset);

      CHECK(mcA->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_A, VoiceGroup::Global}, true)->getValue());
      CHECK(mcB->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_B, VoiceGroup::Global}, true)->getValue());
      CHECK(mcC->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_C, VoiceGroup::Global}, true)->getValue());
      CHECK(mcD->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_D, VoiceGroup::Global}, true)->getValue());

      CHECK(ribbon1ToA->getControlPositionValue() > 0);

      CHECK(ribbon1->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_A, VoiceGroup::Global}, true)->getValue());
      CHECK(ribbon2->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_B, VoiceGroup::Global}, true)->getValue());
      CHECK(ribbon3->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_C, VoiceGroup::Global}, true)->getValue());
      CHECK(ribbon4->getControlPositionValue() == preset->findParameterByID({C15::PID::MC_D, VoiceGroup::Global}, true)->getValue());

      CHECK_FALSE(eb.isModified());

      TestHelper::doMainLoopFor(std::chrono::milliseconds(20));
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "load ribbons of preset 4 correctly")
{
  PresetManagerUseCases pmUseCases(*TestHelper::getPresetManager(), *TestHelper::getSettings());
  auto bank = pmUseCases.importBankFromPath(
      std::filesystem::directory_entry { to_string(getSourceDir())
                                         + std::string("/projects/epc/playground/test-resources/Ribbon_Bug.xml") },
      [](auto s) {});

  WHEN("Preset 4 is Loaded")
  {
    auto& eb = *TestHelper::getEditBuffer();
    auto ribbon1 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_1, VoiceGroup::Global});
    auto ribbon2 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_2, VoiceGroup::Global});
    auto ribbon3 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_3, VoiceGroup::Global});
    auto ribbon4 = eb.findAndCastParameterByID<RibbonParameter>({C15::PID::Ribbon_4, VoiceGroup::Global});

    EditBufferUseCases ebUseCases(eb);

    auto presetNum = 4;

    auto preset = bank->getPresetAt(presetNum);

    const auto ev1 = preset->findParameterByID({C15::PID::MC_A, VoiceGroup::Global}, true)->getValue();
    const auto ev2 = preset->findParameterByID({C15::PID::MC_B, VoiceGroup::Global}, true)->getValue();

    ebUseCases.load(preset);
    CHECK(ribbon1->getControlPositionValue() == ev1);
    CHECK(ribbon2->getControlPositionValue() == ev2);
    CHECK_FALSE(eb.isModified());

    ebUseCases.load(preset);
    CHECK(ribbon1->getControlPositionValue() == ev1);
    CHECK(ribbon2->getControlPositionValue() == ev2);
    CHECK_FALSE(eb.isModified());

    ebUseCases.load(preset);
    CHECK(ribbon1->getControlPositionValue() == ev1);
    CHECK(ribbon2->getControlPositionValue() == ev2);
    CHECK_FALSE(eb.isModified());
  }
}