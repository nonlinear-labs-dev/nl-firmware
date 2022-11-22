#include <catch.hpp>
#include "proxies/audio-engine/AudioEngineProxy.h"
#include "testing/TestHelper.h"
#include "mock/MockPresetStorage.h"

template <typename T> void collectID(const T &t, std::unordered_map<int, int> &counts)
{
  try
  {
    auto &i = counts.at(t.m_id);
    i++;
  }
  catch(...)
  {
    counts[t.m_id] = 1;
  }
}

template <typename T> void collectIDs(const T &range, std::unordered_map<int, int> &counts)
{
  for(auto &t : range)
  {
    collectID(t, counts);
  }
}

template <typename T> void assertMap(const T &t)
{
  for(auto &idCount : t)
  {
    REQUIRE(idCount.second == 1);
  }
}

void assertNoIDTwice(const nltools::msg::SinglePresetMessage &msg)
{
  std::unordered_map<int, int> count;
  collectIDs(msg.m_polyphonicUnmodulateables, count);
  collectIDs(msg.m_polyphonicModulateables, count);
  collectIDs(msg.m_monophonicUnmodulateables[0], count);
  collectIDs(msg.m_monophonicModulateables[0], count);
  collectIDs(msg.m_globalUnmodulateables, count);
  collectIDs(msg.m_globalModulateables, count);
  collectIDs(msg.m_hardwareAmounts, count);
  collectIDs(msg.m_hardwareSources, count);
  collectIDs(msg.m_macroControls, count);
  collectIDs(msg.m_macroTimes, count);
  assertMap(count);
}

template <int vg, typename tMsg> void collectDual(const tMsg &msg)
{
  std::unordered_map<int, int> count;
  collectIDs(msg.m_polyphonicUnmodulateables[vg], count);
  collectIDs(msg.m_polyphonicModulateables[vg], count);
  collectIDs(msg.m_monophonicUnmodulateables[vg], count);
  collectIDs(msg.m_monophonicModulateables[vg], count);
  collectIDs(msg.m_globalUnmodulateables, count);
  collectIDs(msg.m_globalModulateables, count);
  collectIDs(msg.m_hardwareAmounts, count);
  collectIDs(msg.m_hardwareSources, count);
  collectIDs(msg.m_macroControls, count);
  collectIDs(msg.m_macroTimes, count);
  assertMap(count);
}

void assertNoIDTwice(const nltools::msg::SplitPresetMessage &msg)
{
  collectDual<0>(msg);
  collectDual<1>(msg);
}

void assertNoIDTwice(const nltools::msg::LayerPresetMessage &msg)
{
  collectDual<0>(msg);
  collectDual<1>(msg);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Single Preset Message")
{
  MockPresetStorage presets;
  auto single = presets.getSinglePreset();
  auto split = presets.getSplitPreset();
  auto layer = presets.getLayerPreset();
  auto editBuffer = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*editBuffer);

  SECTION("Single Message not duplicate IDs")
  {
    useCase.load(single);
    auto message = AudioEngineProxy::createSingleEditBufferMessage(*editBuffer);
    assertNoIDTwice(message);
  }

  SECTION("Split Message no duplicate IDs")
  {
    useCase.load(split);
    auto message = AudioEngineProxy::createSplitEditBufferMessage(*editBuffer);
    assertNoIDTwice(message);
  }

  SECTION("Layer Message no duplicate IDs")
  {
    useCase.load(layer);
    auto message = AudioEngineProxy::createLayerEditBufferMessage(*editBuffer);
    assertNoIDTwice(message);
  }
}
