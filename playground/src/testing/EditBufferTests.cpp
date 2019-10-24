#include "EditBufferTests.h"
#include <nltools/Testing.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <parameters/PedalParameter.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/AftertouchParameter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/PitchbendParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <parameters/mono-mode-parameters/MonoParameter.h>
#include <nltools/Types.h>

EditBufferTests::EditBufferTests(EditBuffer *eb)
    : m_editBuffer{ eb }
{
  nltools::test::printPassedTests = false;

  initEditBuffer();

  nltools::test::assertEquals(m_editBuffer->getType(), SoundType::Single, "Soundtype is Single");

  const auto originalSinglePresetMessage = AudioEngineProxy::createSingleEditBufferMessage();
  compareSingleSound(m_editBuffer, originalSinglePresetMessage);
  nltools::Log::warning("[PASS] Single Sound Init");


  convertSingleToSplit();

  nltools::test::assertEquals(m_editBuffer->getType(), SoundType::Split, "Soundtype is Split");

  const auto singleToSplitConvertedSound = AudioEngineProxy::createSplitEditBufferMessage();
  compareSplitSoundToSingleOrigin(originalSinglePresetMessage, singleToSplitConvertedSound);
  nltools::Log::warning("[PASS] Split Sound OK!");

  {
    nltools::Log::warning("[TEST] Convert Split to Single");
    auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("[TEST] Convert 2 Single");
    m_editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
  }

  const auto splitToSingleConvertedPresetMessage = AudioEngineProxy::createSingleEditBufferMessage();
  compareSingleSound(splitToSingleConvertedPresetMessage, splitToSingleConvertedPresetMessage);
  nltools::Log::warning("[PASS] Split to Single Sound OK!");

  {
    nltools::Log::warning("[TEST] Convert Single to Layer");
    auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("[TEST] Convert 2 Layer");
    m_editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Layer, VoiceGroup::I);
  }

  const auto layerFromSingleConvertedSound = AudioEngineProxy::createLayerEditBufferMessage();
  compareLayerSoundToOriginalSingle(layerFromSingleConvertedSound, splitToSingleConvertedPresetMessage);
  nltools::Log::warning("[PASS] Layer Sound OK!");

  {
    nltools::Log::warning("[TEST] Convert Layer to Single");
    auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("[TEST] Convert 2 Single");
    m_editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
  }

  const auto singleFromLayerConvertedSound = AudioEngineProxy::createSingleEditBufferMessage();

  compareSingleSound(singleFromLayerConvertedSound, splitToSingleConvertedPresetMessage);

  nltools::Log::warning("[PASS] Edit Buffer tests ran successfully");
}

void EditBufferTests::convertSingleToSplit() const
{
  nltools::Log::warning("[TEST] Convert Single to Split");
  auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("[TEST] Convert Single 2 Split");
  m_editBuffer->undoableConvertToDual(scope->getTransaction(), SoundType::Split, VoiceGroup::I);
}

void EditBufferTests::compareUnmodulateable(const Parameter *parameter,
                                            const nltools::msg::ParameterGroups::Parameter &presetParameter) const
{
  nltools::test::assertNotNull(parameter);
  const auto id1 = parameter->getID();
  const auto id2 = presetParameter.id;

  nltools::test::assertEquals(id1, id2, "ID Equal");

  const auto cp1 = parameter->getControlPositionValue();
  const auto cp2 = presetParameter.controlPosition;

  const auto differs = parameter->isValueDifferentFrom(cp2);
  const auto descr = nltools::string::concat("ID: ", id1, " Value differs, expected: ", cp1, " got: ", cp2);
  nltools::test::_assert(!differs, descr);
}

void EditBufferTests::compareUnmodulateable(const nltools::msg::ParameterGroups::Parameter &p1,
                                            const nltools::msg::ParameterGroups::Parameter &p2) const
{
  nltools::test::assertEquals(p1.id, p2.id, "ID equal");
  nltools::test::assertEquals(p1.controlPosition, p2.controlPosition, "CP equal");
}

void EditBufferTests::compareModulateable(const ModulateableParameter *parameter,
                                          const nltools::msg::ParameterGroups::ModulateableParameter &mod) const
{
  compareUnmodulateable(parameter, mod);
  nltools::test::assertEquals(parameter->getModulationAmount(), mod.modulationAmount, "Modamount Equal");
  nltools::test::assertEquals(parameter->getModulationSource(), mod.mc, "Modsrc Equal");
}

void EditBufferTests::compareModulateable(const nltools::msg::ParameterGroups::ModulateableParameter &p1,
                                          const nltools::msg::ParameterGroups::ModulateableParameter &p2) const
{
  compareUnmodulateable(p1, p2);
  nltools::test::assertEquals(p1.modulationAmount, p2.modulationAmount, "Modamount Equal");
  nltools::test::assertEquals(p1.mc, p2.mc, "Modsrc Equal");
}

void EditBufferTests::compareAfterTouch(const AftertouchParameter *p,
                                        const nltools::msg::ParameterGroups::AftertouchParameter &a) const
{
  compareUnmodulateable(p, a);
  nltools::test::assertEquals(a.returnMode, p->getReturnMode(), "Aftertouch Return Mode Equal");
}

void EditBufferTests::compareAfterTouch(const nltools::msg::ParameterGroups::AftertouchParameter &p1,
                                        const nltools::msg::ParameterGroups::AftertouchParameter &p2) const
{
  compareUnmodulateable(p1, p2);
  nltools::test::assertEquals(p1.returnMode, p2.returnMode, "Aftertouch Return Mode Equal");
}

void EditBufferTests::comparePedal(const PedalParameter *pedal,
                                   const nltools::msg::ParameterGroups::PedalParameter &p) const
{
  compareUnmodulateable(pedal, p);
  nltools::test::assertEquals(p.returnMode, pedal->getReturnMode(), "Pedal Return Mode Equal");
  nltools::test::assertEquals(p.pedalMode, pedal->getPedalMode(), "Pedal Mode Equal");
}

void EditBufferTests::comparePedal(const nltools::msg::ParameterGroups::PedalParameter &p1,
                                   const nltools::msg::ParameterGroups::PedalParameter &p2) const
{
  compareUnmodulateable(p1, p2);
  nltools::test::assertEquals(p1.returnMode, p2.returnMode, "Pedal Return Mode Equal");
  nltools::test::assertEquals(p1.pedalMode, p2.pedalMode, "Pedal Mode Equal");
}

void EditBufferTests::compareRibbon(const RibbonParameter *ribbon,
                                    const nltools::msg::ParameterGroups::RibbonParameter &r) const
{
  compareUnmodulateable(ribbon, r);
  nltools::test::assertEquals(r.ribbonTouchBehaviour, ribbon->getRibbonTouchBehaviour(),
                              "Ribbon Touch Behaviour Equal");
  nltools::test::assertEquals(r.ribbonReturnMode, ribbon->getRibbonReturnMode(), "Ribbon Return Mode Equal");
}

void EditBufferTests::compareRibbon(const nltools::msg::ParameterGroups::RibbonParameter &p1,
                                    const nltools::msg::ParameterGroups::RibbonParameter &p2) const
{
  compareUnmodulateable(p1, p2);
  nltools::test::assertEquals(p1.ribbonReturnMode, p2.ribbonReturnMode, "Ribbon Return Mode Equal");
  nltools::test::assertEquals(p1.ribbonTouchBehaviour, p2.ribbonTouchBehaviour, "Ribbon Touch Behaviour");
}

void EditBufferTests::compareBender(const PitchbendParameter *bender,
                                    const nltools::msg::ParameterGroups::BenderParameter &be) const
{
  compareUnmodulateable(bender, be);
  nltools::test::assertEquals(be.returnMode, bender->getReturnMode(), "Bender Return Mode Equal");
}

void EditBufferTests::compareBender(const nltools::msg::ParameterGroups::BenderParameter &p1,
                                    const nltools::msg::ParameterGroups::BenderParameter &p2) const
{
  compareUnmodulateable(p1, p2);
  nltools::test::assertEquals(p1.returnMode, p2.returnMode, "Bender Return Mode Equal");
}

void EditBufferTests::compareSingleSound(EditBuffer *eb, const nltools::msg::SinglePresetMessage &s) const
{
  {
    const auto ebType = eb->getType();
    const auto msgType = s.type;
    const auto same = ebType == SoundType::Single && msgType == nltools::msg::MessageType::SinglePreset;
    nltools::test::_assert(same, "EditBuffer is not of type Single!");
  }

  for(auto &u : s.unmodulateables)
  {
    auto param = eb->findParameterByID(u.id, VoiceGroup::I);
    nltools::test::assertNotNull(param);
    compareUnmodulateable(param, u);
  }

  for(auto &m : s.modulateables)
  {
    auto mod = dynamic_cast<ModulateableParameter *>(eb->findParameterByID(m.id, VoiceGroup::I));
    nltools::test::assertNotNull(mod);
    compareModulateable(mod, m);
  }

  for(auto &b : s.bender)
  {
    auto bender = dynamic_cast<PitchbendParameter *>(eb->findParameterByID(b.id, VoiceGroup::I));
    nltools::test::assertNotNull(bender);
    compareBender(bender, b);
  }

  for(auto &m : s.monos)
  {
    auto mono = dynamic_cast<MonoParameter *>(eb->findParameterByID(m.id, VoiceGroup::I));
    nltools::test::assertNotNull(mono);
    compareUnmodulateable(mono, m);
  }

  for(auto &mc : s.macros)
  {
    auto macro = dynamic_cast<MacroControlParameter *>(eb->findParameterByID(mc.id, VoiceGroup::I));
    nltools::test::assertNotNull(macro);
    compareUnmodulateable(macro, mc);
  }

  for(auto &a : s.aftertouch)
  {
    auto after = dynamic_cast<AftertouchParameter *>(eb->findParameterByID(a.id, VoiceGroup::I));
    nltools::test::assertNotNull(after);
    compareAfterTouch(after, a);
  }

  for(auto &r : s.ribbons)
  {
    auto ribbon = dynamic_cast<RibbonParameter *>(eb->findParameterByID(r.id, VoiceGroup::I));
    nltools::test::assertNotNull(ribbon);
    compareRibbon(ribbon, r);
  }

  for(auto &p : s.pedals)
  {
    auto pedal = dynamic_cast<PedalParameter *>(eb->findParameterByID(p.id));
    nltools::test::assertNotNull(pedal);
    comparePedal(pedal, p);
  }

  for(auto &m : s.master)
  {
    auto master = eb->findParameterByID(m.id, VoiceGroup::Global);
    nltools::test::assertNotNull(master);
    compareUnmodulateable(master, m);
  }
}

void EditBufferTests::compareSplitSoundToSingleOrigin(const nltools::msg::SinglePresetMessage &original,
                                                      const nltools::msg::SplitPresetMessage &converted)
{
  assertGeneralDualParameterGroups(converted, original);

  for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
  {
    const auto vgIndex = static_cast<int>(vg);

    size_t mono = 0;
    size_t vgMaster = 0;

    for(auto &_ : converted.monos[vgIndex])
    {
      auto &og = original.monos[mono];
      auto &conv = converted.monos[vgIndex][mono];
      mono++;

      compareUnmodulateable(og, conv);
    }

    //Voice Group Master set to global master values
    for(auto &_ : converted.vgMaster[vgIndex])
    {
      auto &og = original.master[vgMaster];
      auto &conv = converted.vgMaster[vgIndex][vgMaster];
      vgMaster++;

      nltools::test::assertEquals(og.controlPosition, conv.controlPosition,
                                  "VG Master equal to previous global Master");
    }
  }

  assertScaleGroupsSame(converted, original);

  //Global Master set to 0 initialy
  size_t master = 0;
  for(auto &_ : converted.master)
  {
    auto &og = original.master[master];
    auto &conv = converted.master[master];
    master++;

    nltools::test::assertEquals(og.id, conv.id, "Global Master ID Same");
    nltools::test::assertEquals(0.0, conv.controlPosition, "Global Master Value initialized to 0");
  }

  nltools::test::assertEquals(converted.splitpoint.controlPosition, 0.5, "Split Point Initialized");
}

void EditBufferTests::compareSingleSound(const nltools::msg::SinglePresetMessage &s,
                                         const nltools::msg::SinglePresetMessage &s1) const
{
  nltools::test::assertEqualsSilent(s, s1, "Single Sound not Equal");
}

void EditBufferTests::compareLayerSoundToOriginalSingle(const nltools::msg::LayerPresetMessage& layer,
                                                        const nltools::msg::SinglePresetMessage& single)
{
  assertGeneralDualParameterGroups(layer, single);

  for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
  {
    const auto vgIndex = static_cast<int>(vg);
    size_t vgMaster = 0;


    for(auto &_ : layer.vgMaster[vgIndex])
    {
      auto &og = single.master[vgMaster];
      auto &conv = layer.vgMaster[vgIndex][vgMaster];
      vgMaster++;

      nltools::test::assertEquals(og.controlPosition, conv.controlPosition, "VG Master equal to prior Global Master");
    }
  }

  size_t master = 0;
  for(auto &_ : layer.master)
  {
    auto &og = single.master[master];
    auto &conv = layer.master[master];
    master++;

    nltools::test::assertEquals(og.id, conv.id, "Global Master ID Equal");
    nltools::test::assertEquals(0.0, conv.controlPosition, "Global Master Value 0");
  }

  assertScaleGroupsSame(layer, single);
}

void EditBufferTests::initEditBuffer()
{
  auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("[TEST] Convert 2 Single");
  m_editBuffer->undoableInitSound(scope->getTransaction());
  m_editBuffer->undoableConvertToSingle(scope->getTransaction(), VoiceGroup::I);
}
