#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include "parameters/RibbonParameter.h"
#include <presets/Bank.h>
#include <presets/Preset.h>

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

    EditBufferUseCases ebUseCases(eb);
    ebUseCases.load(bank->getPresetAt(0));

    CHECK(ribbon1->getControlPositionValue() == Approx(0.66));
    CHECK(ribbon2->getControlPositionValue() == Approx(0.4));
    CHECK(ribbon3->getControlPositionValue() == Approx(0.33));
    CHECK(ribbon4->getControlPositionValue() == Approx(0.2));

    //load again
    ebUseCases.load(bank->getPresetAt(0));

    CHECK(ribbon1->getControlPositionValue() == Approx(0.66));
    CHECK(ribbon2->getControlPositionValue() == Approx(0.4));
    CHECK(ribbon3->getControlPositionValue() == Approx(0.33));
    CHECK(ribbon4->getControlPositionValue() == Approx(0.2));
  }

}