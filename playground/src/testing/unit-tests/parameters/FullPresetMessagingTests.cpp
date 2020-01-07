#include <third-party/include/catch.hpp>
#include <presets/Preset.h>
#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <presets/PresetParameter.h>

#include <parameters/PhysicalControlParameter.h>
#include <parameters/ModulationRoutingParameter.h>
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <thread>
#include <testing/unit-tests/mock/PresetSenderAndReceiverWithTest.h>
#include <nltools/Types.h>
#include <groups/HardwareSourcesGroup.h>
#include <presets/EditBuffer.h>

/** to test:
 *  modamount in preset message
 */

void setPresetValuesLoadAndSendVerify(EditBuffer* eb, Preset* preset, tControlPositionValue cp,
                                      tControlPositionValue modAmt, MacroControls mc);

TEST_CASE("msg parameter operators")
{
  nltools::msg::ParameterGroups::UnmodulateableParameter param;
  param.controlPosition = 2.3;
  param.id = 187;

  REQUIRE(param == param);

  nltools::msg::ParameterGroups::ModulateableParameter modparam;
  modparam.controlPosition = 4.20;
  modparam.id = 420;
  modparam.mc = MacroControls::MC1;
  modparam.modulationAmount = 0.187;
  REQUIRE(modparam == modparam);

  REQUIRE_FALSE(modparam == param);

  auto single = AudioEngineProxy::createSingleEditBufferMessage();
  REQUIRE(single == single);
}

TEST_CASE("working simple tests single preset with 0 and 1 respectively")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;

  Preset* presetWithAllZeros = nullptr;
  Preset* presetWithAllOnes = nullptr;

  {
    auto scope = TestHelper::createTestScope();
    presetWithAllOnes = presets.createSinglePreset(scope->getTransaction());
    presetWithAllZeros = presets.createSinglePreset(scope->getTransaction());
  }

  REQUIRE(presetWithAllOnes);
  REQUIRE(presetWithAllZeros);

  {
    auto scope = TestHelper::createTestScope();
    presetWithAllOnes->forEachParameter([&](PresetParameter* p) { p->setValue(scope->getTransaction(), 1.0); });
    presetWithAllZeros->forEachParameter([&](PresetParameter* p) { p->setValue(scope->getTransaction(), 0); });
  }

  {
    auto allOne = true;
    auto allZero = true;
    presetWithAllOnes->forEachParameter([&](PresetParameter* p) {
      if(p->getValue() != 1)
        allOne = false;
    });
    presetWithAllZeros->forEachParameter([&](PresetParameter* p) {
      if(p->getValue() != 0)
        allZero = false;
    });
    REQUIRE(allOne);
    REQUIRE(allZero);
  }

  for(auto presetWithValue :
      std::vector<std::pair<Preset*, double>>{ { presetWithAllOnes, 1 }, { presetWithAllZeros, 0 } })
  {
    auto scope = TestHelper::createTestScope();
    eb->undoableLoad(scope->getTransaction(), presetWithValue.first);
    eb->forEachParameter([&](const Parameter* p) {
      auto expectedValue = presetWithValue.second;
      auto isExpected = p->getValue().getRawValue() == expectedValue;

      auto shouldHaveExpected
          = !(dynamic_cast<const PhysicalControlParameter*>(p) || dynamic_cast<const ModulationRoutingParameter*>(p));

      if(shouldHaveExpected && !isExpected)
        nltools::Log::error(p->getLongName(), "is not expected but", p->getValue().getRawValue());

      if(shouldHaveExpected)
        REQUIRE(isExpected);
    });
  }
}

TEST_CASE("test messages get send accordingly")
{
  using namespace nltools::msg;

  PresetSenderAndReceiverWithTest<SinglePresetMessage> t1;
  PresetSenderAndReceiverWithTest<SplitPresetMessage> t2;
  PresetSenderAndReceiverWithTest<LayerPresetMessage> t3;
}

TEST_CASE("Load Preset Works!")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;

  SECTION("validate single preset loading")
  {
    setPresetValuesLoadAndSendVerify(eb, presets.getSinglePreset(), 0.16, 0.2, MacroControls::MC5);
  }

  SECTION("validate layer preset loading")
  {
    setPresetValuesLoadAndSendVerify(eb, presets.getLayerPreset(), 0.45, 0.67, MacroControls::MC1);
  }
}

bool isControlPositionCorrect(Parameter* p, tControlPositionValue expected)
{
  auto cp = p->getControlPositionValue();
  auto fine = p->getValue().getFineDenominator();

  auto stepSize = 1.0 / fine;
  return std::abs(cp - expected) <= stepSize;
}

void setPresetValuesLoadAndSendVerify(EditBuffer* eb, Preset* preset, tControlPositionValue cp,
                                      tControlPositionValue modAmt, MacroControls mc)
{

  {
    auto scope = TestHelper::createTestScope();
    auto trans = scope->getTransaction();

    preset->forEachParameter([&](PresetParameter* pp) {
      pp->setValue(trans, cp);
      if(auto realParameter = eb->findParameterByID(pp->getID()))
      {
        if(auto modParam = dynamic_cast<ModulateableParameter*>(realParameter))
        {
          pp->setField(trans, PresetParameter::Fields::ModAmount, std::to_string(modAmt));
          pp->setField(trans, PresetParameter::Fields::ModSource, std::to_string(static_cast<int>(mc)));
        }
      }
    });
  }

  preset->forEachParameter([&](PresetParameter* pp) {
    WHEN(pp->getID().toString() + " kept preset value")
    {
      REQUIRE(pp->getValue() == cp);
    }
  });

  THEN("load and verify")
  {
    {
      auto scope = TestHelper::createTestScope();
      REQUIRE_FALSE(eb->hasLocks(VoiceGroup::I));
      REQUIRE_FALSE(eb->hasLocks(VoiceGroup::II));
      REQUIRE_FALSE(eb->hasLocks(VoiceGroup::Global));
      eb->undoableLoad(scope->getTransaction(), preset);
    }

    eb->forEachParameter([modAmt, mc, cp](Parameter* p) {
      if(dynamic_cast<HardwareSourcesGroup*>(p->getParent()))
      {
        auto ret = isControlPositionCorrect(p, cp);

        if(!ret)
          nltools::Log::error(p->getID().toString()
                              + " did not load! got: " + std::to_string(p->getControlPositionValue()));

        REQUIRE(isControlPositionCorrect(p, cp));

        if(auto modParam = dynamic_cast<ModulateableParameter*>(p))
        {
          REQUIRE(modParam->getModulationAmount() == modAmt);
          REQUIRE(modParam->getModulationSource() == mc);
        }
      }
    });
  }
}