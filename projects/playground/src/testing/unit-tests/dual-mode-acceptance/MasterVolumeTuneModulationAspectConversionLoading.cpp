#include <testing/TestHelper.h>
#include <presets/Preset.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <presets/PresetParameter.h>
#include <parameters/ModulateableParameter.h>

namespace detail
{
  void initPreset(UNDO::Transaction* transaction, Preset* p)
  {
    auto presetMasterVolume = p->findParameterByID({ 247, VoiceGroup::Global }, false);
    auto presetMasterTune = p->findParameterByID({ 248, VoiceGroup::Global }, false);

    presetMasterVolume->setField(transaction, PresetParameter::Fields::ModAmount, "0.5");
    presetMasterVolume->setField(transaction, PresetParameter::Fields::ModSource, "1");

    presetMasterTune->setField(transaction, PresetParameter::Fields::ModAmount, "0.5");
    presetMasterTune->setField(transaction, PresetParameter::Fields::ModSource, "1");
  }
}

TEST_CASE("Load Single Preset into Part copies Master Mod Aspects to Part")
{
  MockPresetStorage presets;
  auto singlePreset = presets.getSinglePreset();
  auto layerPreset = presets.getLayerPreset();

  auto masterVolume = ParameterId { 247, VoiceGroup::Global };
  auto masterTune = ParameterId { 248, VoiceGroup::Global };

  auto tuneI = ParameterId { 360, VoiceGroup::I };
  auto tuneII = ParameterId { 360, VoiceGroup::II };
  auto volumeI = ParameterId { 358, VoiceGroup::I };
  auto volumeII = ParameterId { 358, VoiceGroup::II };

  auto eb = TestHelper::getEditBuffer();

  auto modMasterVolume = dynamic_cast<ModulateableParameter*>(eb->findParameterByID(masterVolume));
  auto modMasterTune = dynamic_cast<ModulateableParameter*>(eb->findParameterByID(masterTune));
  auto modIVolume = dynamic_cast<ModulateableParameter*>(eb->findParameterByID(volumeI));
  auto modIIVolume = dynamic_cast<ModulateableParameter*>(eb->findParameterByID(volumeII));
  auto modITune = dynamic_cast<ModulateableParameter*>(eb->findParameterByID(tuneI));
  auto modIITune = dynamic_cast<ModulateableParameter*>(eb->findParameterByID(tuneII));

  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    detail::initPreset(transaction, singlePreset);
    detail::initPreset(transaction, layerPreset);

    TestHelper::initDualEditBuffer<SoundType::Split>(transaction);

    CHECK(modMasterVolume->getModulationSource() == MacroControls::NONE);
    CHECK(modMasterVolume->getModulationAmount() == 0);

    CHECK(modMasterTune->getModulationSource() == MacroControls::NONE);
    CHECK(modMasterTune->getModulationAmount() == 0);

    CHECK(modIVolume->getModulationSource() == MacroControls::NONE);
    CHECK(modIVolume->getModulationAmount() == 0);

    CHECK(modIIVolume->getModulationSource() == MacroControls::NONE);
    CHECK(modIIVolume->getModulationAmount() == 0);

    CHECK(modITune->getModulationSource() == MacroControls::NONE);
    CHECK(modITune->getModulationAmount() == 0);

    CHECK(modIITune->getModulationSource() == MacroControls::NONE);
    CHECK(modIITune->getModulationAmount() == 0);
  }

  THEN("Load Single into I")
  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoadPresetIntoDualSound(transaction, singlePreset, VoiceGroup::I);

    CHECK(modITune->getModulationSource() == MacroControls::MC1);
    CHECK(modITune->getModulationAmount() == 0.5);

    CHECK(modIVolume->getModulationSource() == MacroControls::MC1);
    CHECK(modIVolume->getModulationAmount() == 0.5);
  }

  THEN("Load Single and Convert to Dual")
  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoad(transaction, singlePreset);

    CHECK(modMasterTune->getModulationSource() == MacroControls::MC1);
    CHECK(modMasterTune->getModulationAmount() == 0.5);

    CHECK(modMasterVolume->getModulationSource() == MacroControls::MC1);
    CHECK(modMasterVolume->getModulationAmount() == 0.5);

    eb->undoableConvertToDual(transaction, SoundType::Layer);

    //check part master have aspects
    CHECK(modITune->getModulationAmount() == 0.5);
    CHECK(modITune->getModulationSource() == MacroControls::MC1);
    CHECK(modIITune->getModulationAmount() == 0.5);
    CHECK(modIITune->getModulationSource() == MacroControls::MC1);

    CHECK(modIVolume->getModulationAmount() == 0.5);
    CHECK(modIVolume->getModulationSource() == MacroControls::MC1);
    CHECK(modIIVolume->getModulationAmount() == 0.5);
    CHECK(modIIVolume->getModulationSource() == MacroControls::MC1);

    //Check Global Master Apects have reset
    CHECK(modMasterTune->getModulationSource() == MacroControls::NONE);
    CHECK(modMasterTune->getModulationAmount() == 0.0);

    CHECK(modMasterVolume->getModulationSource() == MacroControls::NONE);
    CHECK(modMasterVolume->getModulationAmount() == 0.0);
  }

  THEN("Convert to Dual and load Part of Dual Preset ignore Global Master Volume Tune of preset")
  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableConvertToDual(transaction, SoundType::Layer);

    CHECK(modMasterTune->getModulationSource() == MacroControls::NONE);
    CHECK(modMasterTune->getModulationAmount() == 0.0);

    CHECK(modMasterVolume->getModulationSource() == MacroControls::NONE);
    CHECK(modMasterVolume->getModulationAmount() == 0.0);

    const auto globalGroupHash = modMasterTune->getParentGroup()->getHash();

    eb->undoableLoadPresetPartIntoPart(transaction, presets.getLayerPreset(), VoiceGroup::I, VoiceGroup::I);

    const auto afterGlobalHash = modMasterTune->getParentGroup()->getHash();

    CHECK(globalGroupHash == afterGlobalHash);
  }

  THEN("Convert Dual into Single copies part master aspects to global parameters")
  {
    auto scope = TestHelper::createTestScope();
    auto transaction = scope->getTransaction();

    eb->undoableLoad(transaction, layerPreset);

    eb->undoableConvertToSingle(transaction, VoiceGroup::I);

    CHECK(modMasterTune->getModulationSource() == MacroControls::MC1);
    CHECK(modMasterTune->getModulationAmount() == 0.5);

    CHECK(modMasterVolume->getModulationSource() == MacroControls::MC1);
    CHECK(modMasterVolume->getModulationAmount() == 0.5);
  }
}