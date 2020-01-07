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

/** to test:
 *  modamount in preset message
 */

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

TEST_CASE("test messages get send accordingly")
{
  auto eb = TestHelper::getEditBuffer();
  MockPresetStorage presets;

  Preset* presetWithAllZeros = nullptr;
  Preset* presetWithAllOnes = nullptr;

  {
    auto scope = TestHelper::createTestScope();
    presetWithAllOnes = MockPresetStorage::createSinglePreset(scope->getTransaction());
    presetWithAllZeros = MockPresetStorage::createSinglePreset(scope->getTransaction());
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

  {
    using namespace nltools::msg;
    auto singleMessage = AudioEngineProxy::createSingleEditBufferMessage();

    PresetSenderAndReceiverWithTest<SinglePresetMessage> t1(
        [&](const SinglePresetMessage& received) {
          auto ret = received == singleMessage;
          REQUIRE(ret);
          return ret;
        },
        [&]() { send(EndPoint::TestEndPoint, singleMessage); });

    auto splitMessage = AudioEngineProxy::createSplitEditBufferMessage();

    PresetSenderAndReceiverWithTest<SplitPresetMessage> t2(
        [&](const SplitPresetMessage& received) {
          auto ret = received == splitMessage;
          REQUIRE(ret);
          return ret;
        },
        [&]() { send(EndPoint::TestEndPoint, splitMessage); });

    auto layerMessage = AudioEngineProxy::createLayerEditBufferMessage();

    PresetSenderAndReceiverWithTest<LayerPresetMessage> t3(
        [&](const LayerPresetMessage& received) {
          auto ret = received == layerMessage;
          REQUIRE(ret);
          return ret;
        },
        [&]() { send(EndPoint::TestEndPoint, layerMessage); });
  }
}