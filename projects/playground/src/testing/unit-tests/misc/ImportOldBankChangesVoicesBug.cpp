#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/FuxieSwarmsTestBank.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <parameters/scale-converters/Linear12CountScaleConverter.h>
#include <parameters/scale-converters/LinearCountScaleConverter.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>
#include <http/UpdateDocumentMaster.h>
#include <xml/MemoryInStream.h>
#include <libundo/undo/Scope.h>

TEST_CASE("Import Bank Fresh Results in Correct Voices", "[Unison]")
{
  FuxieSwarmsTestBank bank;
  auto preset = bank.getPreset(0);
  auto eb = TestHelper::getEditBuffer();

  {
    auto scope = TestHelper::createTestScope();
    TestHelper::initSingleEditBuffer(scope->getTransaction());
  }

  WHEN("Unison Voices Correct")
  {
    PresetParameter* unisonVoices = nullptr;
    CHECK_NOTHROW(unisonVoices = preset->findParameterByID({ 249, VoiceGroup::I }, true));
    CHECK(unisonVoices);

    const auto ogValue = unisonVoices->getValue();
    auto voicesI = eb->findParameterByID({ 249, VoiceGroup::I });
    CHECK(voicesI->getValue().getDisplayString(ogValue) == "8 voices");
    CHECK(voicesI->getValue().getDisplayString() == "1 voice (off)");

    THEN("Load Preset -> voices == 8")
    {
      auto scope = TestHelper::createTestScope();
      auto transaction = scope->getTransaction();
      eb->undoableLoad(transaction, preset);

      CHECK(eb->getType() == SoundType::Single);
      CHECK(voicesI->getDisplayString() == "8 voices");
    }

    THEN("Load into Dual -> voices == 8")
    {
      auto scope = TestHelper::createTestScope();
      auto transaction = scope->getTransaction();
      TestHelper::initDualEditBuffer<SoundType::Layer>(transaction);
      eb->undoableLoadPresetPartIntoPart(transaction, preset, VoiceGroup::I, VoiceGroup::I);

      CHECK(eb->getType() == SoundType::Layer);
      CHECK(voicesI->getDisplayString() == "8 voices");
    }
  }
}

TEST_CASE("UnisonVoices Conversion rules")
{
  class UpdateDocumentMasterMock : public UpdateDocumentMaster
  {
   public:
    void writeDocument(Writer&, tUpdateID) const override
    {
    }
  };

  UpdateDocumentMasterMock root;
  PresetManager pm(&root);
  Bank bank(&pm);

  auto readXml = [&](auto xml) {
    MemoryInStream stream(xml, false);
    UNDO::Scope scope(&root);
    auto transactionScope = scope.startTransaction("foo");
    XmlReader reader(stream, transactionScope->getTransaction());
    reader.read<PresetBankSerializer>(&bank);
  };

  WHEN("reading version 5")
  {
    THEN("minimum is 0 => 1 voice")
    {
      readXml("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
              "<bank version=\"5\">"
              "   <preset-order><uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid></preset-order>"
              "   <preset pos=\"0\">"
              "       <uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid>"
              "       <parameter-groups>"
              "           <parameter-group id=\"Unison\">"
              "               <parameter id=\"249\"><value>0</value></parameter>"
              "           </parameter-group>"
              "       </parameter-groups>"
              "   </preset>"
              "</bank>");
      auto p = bank.findPreset("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa")->findParameterByID({ 249, VoiceGroup::I }, true);
      CHECK(p->getValue() == 0);
    }

    THEN("maximum is 11/23 => 12 voices")
    {
      readXml("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
              "<bank version=\"5\">"
              "   <preset-order><uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid></preset-order>"
              "   <preset pos=\"0\">"
              "       <uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid>"
              "       <parameter-groups>"
              "           <parameter-group id=\"Unison\">"
              "               <parameter id=\"249\"><value>1</value></parameter>"
              "           </parameter-group>"
              "       </parameter-groups>"
              "   </preset>"
              "</bank>");
      auto p = bank.findPreset("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa")->findParameterByID({ 249, VoiceGroup::I }, true);
      CHECK(p->getValue() == 11 / 23.0);
    }
  }

  WHEN("reading version 7")
  {
    THEN("maximum for single preset is 23 => 24 voices")
    {
      readXml("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
              "<bank version=\"7\">"
              "   <preset-order><uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid></preset-order>"
              "   <preset pos=\"0\">"
              "       <uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid>"
              "       <type>Single</type>"
              "       <parameter-groups>"
              "           <parameter-group id=\"I-Unison\">"
              "               <parameter id=\"I-249\"><value>1</value></parameter>"
              "           </parameter-group>"
              "       </parameter-groups>"
              "   </preset>"
              "</bank>");
      auto p = bank.findPreset("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa")->findParameterByID({ 249, VoiceGroup::I }, true);
      CHECK(p->getValue() == 1.0);
    }

    THEN("maximum for split preset is 11 => 12 voices")
    {
      readXml("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
              "<bank version=\"7\">"
              "   <preset-order><uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid></preset-order>"
              "   <preset pos=\"0\">"
              "       <uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid>"
              "       <type>Split</type>"
              "       <parameter-groups>"
              "           <parameter-group id=\"I-Unison\">"
              "               <parameter id=\"I-249\"><value>1</value></parameter>"
              "           </parameter-group>"
              "       </parameter-groups>"
              "   </preset>"
              "</bank>");
      auto p = bank.findPreset("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa")->findParameterByID({ 249, VoiceGroup::I }, true);
      CHECK(p->getValue() == 11 / 23.0);
    }

    THEN("maximum for dual preset is 11 => 12 voices")
    {
      readXml("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
              "<bank version=\"7\">"
              "   <preset-order><uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid></preset-order>"
              "   <preset pos=\"0\">"
              "       <uuid>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</uuid>"
              "       <type>Layer</type>"
              "       <parameter-groups>"
              "           <parameter-group id=\"I-Unison\">"
              "               <parameter id=\"I-249\"><value>1</value></parameter>"
              "           </parameter-group>"
              "       </parameter-groups>"
              "   </preset>"
              "</bank>");
      auto p = bank.findPreset("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa")->findParameterByID({ 249, VoiceGroup::I }, true);
      CHECK(p->getValue() == 11 / 23.0);
    }
  }
}
