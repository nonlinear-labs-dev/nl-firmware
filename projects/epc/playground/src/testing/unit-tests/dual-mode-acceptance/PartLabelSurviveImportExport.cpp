#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <presets/Preset.h>
#include <presets/PresetManager.h>
#include <xml/FileOutStream.h>
#include <xml/VersionAttribute.h>
#include <presets/BankActions.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Export/Import Presets With Part Names", "[Preset][Store][Export][Import]")
{
  auto pm = TestHelper::getPresetManager();

  auto setVGName
      = [](auto trans, Preset* p, VoiceGroup vg, const char* n) { p->undoableSetVoiceGroupName(trans, vg, n); };

  MockPresetStorage presets;

  auto singlePreset = presets.getSinglePreset();
  auto layerPreset = presets.getLayerPreset();

  auto bank = static_cast<Bank*>(singlePreset->getParent());

  {
    auto scope = TestHelper::createTestScope();
    auto trans = scope->getTransaction();

    setVGName(trans, singlePreset, VoiceGroup::I, "I");
    setVGName(trans, singlePreset, VoiceGroup::II, "II");

    setVGName(trans, layerPreset, VoiceGroup::I, "I");
    setVGName(trans, layerPreset, VoiceGroup::II, "II");
  }

  auto CHECK_LABELS_PRESENT = [](const Preset* p) {
    CHECK(p->getVoiceGroupName(VoiceGroup::I) == "I");
    CHECK(p->getVoiceGroupName(VoiceGroup::II) == "II");
  };

  THEN("Export Bank -> Import Bank -> Check Labels")
  {
    CHECK_LABELS_PRESENT(singlePreset);
    CHECK_LABELS_PRESENT(layerPreset);

    auto oldNumPresets = bank->getNumPresets();
    auto oldBankName = bank->getName(false);

    {
      PresetBankSerializer serializer(bank, {}, false);
      FileOutStream stream("/tmp/testbank.xml", false);
      XmlWriter writer(stream);
      serializer.write(writer, VersionAttribute::get());
    }

    auto newBank = [&] {
      auto settings = TestHelper::getSettings();
      FileInStream stream("/tmp/testbank.xml", false);
      PresetManagerUseCases useCase(*TestHelper::getPresetManager(), *settings);
      return useCase.importBankFromStream(stream, 0, 0, "/tmp/testbank.xml", [](auto){});
    }();

    CHECK_LABELS_PRESENT(newBank->getPresetAt(0));
    CHECK_LABELS_PRESENT(newBank->getPresetAt(1));

    CHECK(newBank->getNumPresets() == oldNumPresets);
    CHECK(newBank->getName(false) == oldBankName);

    auto settings = TestHelper::getSettings();
    PresetManagerUseCases useCase(*pm, *settings);
    useCase.deleteBank(newBank);
  }
}
