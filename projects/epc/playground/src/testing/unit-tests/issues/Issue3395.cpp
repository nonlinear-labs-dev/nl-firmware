#include <testing/TestHelper.h>
#include "testing/unit-tests/mock/MockPresetStorage.h"
#include "use-cases/PresetUseCases.h"
#include <presets/Preset.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "issue 3395")
{
  MockPresetStorage presets;
  auto preset = presets.getSinglePreset();

  PresetUseCases presetUC(*preset, *TestHelper::getSettings());
  PresetManagerUseCases pmUC(*TestHelper::getPresetManager(), *TestHelper::getSettings());
  EditBufferUseCases ebUC(*TestHelper::getEditBuffer());

  WHEN("preset has comment and is loaded")
  {
    presetUC.setComment("preset comment");
    presetUC.setAttribute("color", "red");
    ebUC.load(preset);

    CHECK(TestHelper::getEditBuffer()->getAttribute("Comment", "") == "preset comment");
    CHECK(TestHelper::getEditBuffer()->getAttribute("color", "") == "red");

    THEN("init sound is pressed")
    {
      ebUC.initSound(Defaults::FactoryDefault);

      WHEN("new Preset is stored")
      {
        auto newBank = pmUC.createBankAndStoreEditBuffer();
        auto newPreset = newBank->getPresetAt(0);

        REQUIRE(newPreset != nullptr);

        THEN("Preset Comment is empty")
        {
          CHECK(newPreset->getComment().empty());
          CHECK(newPreset->getComment() != "preset comment");
        }

        THEN("color tag is none")
        {
          CHECK(newPreset->getAttribute("color", "").empty());
        }
      }
    }
  }
}
