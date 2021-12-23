#include <testing/TestHelper.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <parameter_declarations.h>
#include <use-cases/PresetUseCases.h>

TEST_CASE("Store Preset will change EditBuffer Origin and 'mark saved Preset as loaded'")
{
  auto eb = TestHelper::getEditBuffer();
  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();

  PresetManagerUseCases pmUseCases(*pm, *settings);
  EditBufferUseCases ebUseCases(*eb);


  WHEN("New Bank created")
  {
    auto bank = pmUseCases.newBank("0", "0", std::nullopt);
    REQUIRE(bank != nullptr);
    REQUIRE(!bank->hasPresets());

    BankUseCases bankUseCase(bank, *TestHelper::getSettings());

    WHEN("EditBuffer is prepared")
    {
      using namespace std::chrono_literals;
      ebUseCases.initSound(Defaults::UserDefault);
      REQUIRE(!eb->isModified());
      REQUIRE(!eb->findAnyParameterChanged());
      ebUseCases.setParameter({C15::PID::Env_A_Att, VoiceGroup::I}, 0.187);
      TestHelper::doMainLoopFor(150ms);
      REQUIRE(eb->isModified());
      REQUIRE(eb->findAnyParameterChanged());

      WHEN("Changed EB is Appended")
      {
        auto preset = bankUseCase.appendEditBuffer();
        TestHelper::doMainLoopFor(150ms);
        CHECK(preset != nullptr);
        CHECK(eb->getOrigin() == preset);
        CHECK(!eb->isModified());
        CHECK(!eb->findAnyParameterChanged());
      }

      WHEN("Changed EB is Inserted")
      {
        auto preset = bankUseCase.insertEditBufferAtPosition(0);
        TestHelper::doMainLoopFor(150ms);
        CHECK(preset != nullptr);
        CHECK(eb->getOrigin() == preset);
        CHECK(eb->getUUIDOfLastLoadedPreset() == preset->getUuid());
        CHECK(!eb->isModified());
        CHECK(!eb->findAnyParameterChanged());

        WHEN("EB is Changed again")
        {
          ebUseCases.setParameter({C15::PID::Voice_Grp_Tune, VoiceGroup::I}, 0.267);
          TestHelper::doMainLoopFor(150ms);
          CHECK(eb->isModified());
          CHECK(eb->findAnyParameterChanged());

          THEN("Preset is overwritten with changed EB")
          {
            PresetUseCases presetUseCase(*preset, *TestHelper::getSettings());
            presetUseCase.overwriteWithEditBuffer(*eb);
            TestHelper::doMainLoopFor(150ms);
            CHECK(eb->getOrigin() == preset);
            CHECK(!eb->isModified());
            CHECK(!eb->findAnyParameterChanged());
            CHECK(eb->getUUIDOfLastLoadedPreset() == preset->getUuid());
          }
        }
      }
    }
  }
}