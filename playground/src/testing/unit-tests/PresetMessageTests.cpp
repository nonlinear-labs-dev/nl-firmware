#include "third-party/include/catch.hpp"
#include "proxies/audio-engine/AudioEngineProxy.h"
#include "presets/EditBuffer.h"
#include "testing/TestHelper.h"
#include "MockPresetStorage.h"

template <typename T> void collectID(const T &t, std::unordered_map<int, int> &counts)
{
  try
  {
    auto &i = counts.at(t.id);
    i++;
  }
  catch(...)
  {
    counts[t.id] = 1;
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
  collectID(msg.unisonVoices, count);
  collectIDs(msg.unmodulateables, count);
  collectIDs(msg.modulateables, count);
  collectIDs(msg.hwamounts, count);
  collectIDs(msg.hwsources, count);
  collectIDs(msg.macros, count);
  collectIDs(msg.globalparams, count);
  assertMap(count);
}

template <int vg, typename tMsg> void collectDual(const tMsg &msg)
{
  std::unordered_map<int, int> count;
  collectID(msg.unisonVoices[vg], count);
  collectIDs(msg.unmodulateables[vg], count);
  collectIDs(msg.modulateables[vg], count);
  collectIDs(msg.hwamounts, count);
  collectIDs(msg.hwsources, count);
  collectIDs(msg.macros, count);
  collectIDs(msg.globalparams, count);
  collectID(msg.splitpoint, count);
  assertMap(count);
}

template <int vg> void collectDual(const nltools::msg::LayerPresetMessage &msg)
{
  std::unordered_map<int, int> count;
  collectID(msg.unisonVoices[vg], count);
  collectIDs(msg.unmodulateables[vg], count);
  collectIDs(msg.modulateables[vg], count);
  collectIDs(msg.hwamounts, count);
  collectIDs(msg.hwsources, count);
  collectIDs(msg.macros, count);
  collectIDs(msg.globalparams, count);
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

TEST_CASE("Single Preset Message")
{
  MockPresetStorage presets;
  auto editBuffer = TestHelper::getEditBuffer();

  SECTION("Single Message not duplicate IDs")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSinglePreset());
    auto message = AudioEngineProxy::createSingleEditBufferMessage();
    assertNoIDTwice(message);
  }

  SECTION("Split Message no duplicate IDs")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getSplitPreset());
    auto message = AudioEngineProxy::createSplitEditBufferMessage();
    assertNoIDTwice(message);
  }

  SECTION("Layer Message no duplicate IDs")
  {
    auto scope = TestHelper::createTestScope();
    editBuffer->undoableLoad(scope->getTransaction(), presets.getLayerPreset());
    auto message = AudioEngineProxy::createLayerEditBufferMessage();
    assertNoIDTwice(message);
  }
}
