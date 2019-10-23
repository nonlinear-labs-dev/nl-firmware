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

EditBufferTests::EditBufferTests(EditBuffer *eb)
    : m_editBuffer{ eb }
{
  nltools::Log::warning("Starting Edit Buffer tests");
  {
    auto scope = m_editBuffer->getParent()->getUndoScope().startTransaction("[TEST] Convert 2 Single");
    m_editBuffer->undoableConvertToSingle(scope->getTransaction());
  }

  const auto originalSinglePresetMessage = AudioEngineProxy::createSingleEditBufferMessage();
  compareSingleSound(m_editBuffer, originalSinglePresetMessage);

  nltools::Log::warning("Edit Buffer tests ran successfully");
}

void EditBufferTests::compareUnmodulateable(const Parameter *parameter,
                                            const nltools::msg::ParameterGroups::Parameter &presetParameter) const
{
  nltools::test::_assert(parameter != nullptr, "Parameter is null!");
  const auto id1 = parameter->getID();
  const auto id2 = presetParameter.id;

  nltools::test::_assert(id1 == id2, nltools::string::concat("ID differs! expected: ", id1, " got ", id2));

  const auto cp1 = parameter->getControlPositionValue();
  const auto cp2 = presetParameter.controlPosition;

  const auto differs = parameter->isValueDifferentFrom(cp2);
  const auto descr = nltools::string::concat("ID: ", id1, " Value differs, expected: ", cp1, " got: ", cp2);
  nltools::test::_assert(!differs, descr);
}

void EditBufferTests::compareModulateable(const ModulateableParameter *parameter,
                                          const nltools::msg::ParameterGroups::ModulateableParameter &mod) const
{
  compareUnmodulateable(parameter, mod);
  nltools::test::_assert(parameter->getModulationAmount() == static_cast<double>(mod.modulationAmount),
                         "ModAmount differs");
  nltools::test::_assert(parameter->getModulationSource() == mod.mc, "Mod Source differs");
}

void EditBufferTests::compareAfterTouch(const AftertouchParameter *p,
                                        const nltools::msg::ParameterGroups::AftertouchParameter &a) const
{
  compareUnmodulateable(p, a);
  nltools::test::_assert(a.returnMode == p->getReturnMode(), "Returnmode differs");
}

void EditBufferTests::comparePedal(const PedalParameter *pedal,
                                   const nltools::msg::ParameterGroups::PedalParameter &p) const
{
  compareUnmodulateable(pedal, p);
  nltools::test::_assert(p.returnMode == pedal->getReturnMode(), "Return mode differs");
  nltools::test::_assert(p.pedalMode == pedal->getPedalMode(), "Pedal mode differs");
}

void EditBufferTests::compareRibbon(const RibbonParameter *ribbon,
                                    const nltools::msg::ParameterGroups::RibbonParameter &r) const
{
  compareUnmodulateable(ribbon, r);
  nltools::test::_assert(r.ribbonTouchBehaviour == ribbon->getRibbonTouchBehaviour(), "Touch behaviour differs");
  nltools::test::_assert(r.ribbonReturnMode == ribbon->getRibbonReturnMode(), "Ribbon Returnmode differs");
}

void EditBufferTests::compareBender(const PitchbendParameter *bender,
                                    const nltools::msg::ParameterGroups::BenderParameter &be) const
{
  compareUnmodulateable(bender, be);
  nltools::test::_assert(be.returnMode == bender->getReturnMode(), "Returnmode differs");
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
    nltools::test::_assert(param != nullptr, "Parameter not found!");
    compareUnmodulateable(param, u);
  }

  for(auto &m : s.modulateables)
  {
    auto mod = dynamic_cast<ModulateableParameter *>(eb->findParameterByID(m.id, VoiceGroup::I));
    nltools::test::_assert(mod != nullptr, "Parameter is not Modulateable!");
    compareModulateable(mod, m);
  }

  for(auto &b : s.bender)
  {
    auto bender = dynamic_cast<PitchbendParameter *>(eb->findParameterByID(b.id, VoiceGroup::I));
    nltools::test::_assert(bender != nullptr, "Parameter is not Pitchbender!");
    compareBender(bender, b);
  }

  for(auto &m : s.monos)
  {
    auto mono = dynamic_cast<MonoParameter *>(eb->findParameterByID(m.id, VoiceGroup::I));
    nltools::test::_assert(mono != nullptr, "Parameter is not MonoParameter!");
    compareUnmodulateable(mono, m);
  }

  for(auto &mc : s.macros)
  {
    auto macro = dynamic_cast<MacroControlParameter *>(eb->findParameterByID(mc.id, VoiceGroup::I));
    nltools::test::_assert(macro != nullptr, "Parameter is not MacroControl!");
    compareUnmodulateable(macro, mc);
  }

  for(auto &a : s.aftertouch)
  {
    auto after = dynamic_cast<AftertouchParameter *>(eb->findParameterByID(a.id, VoiceGroup::I));
    nltools::test::_assert(after != nullptr, "Parameter not Aftertouch!");
    compareAfterTouch(after, a);
  }

  for(auto &r : s.ribbons)
  {
    auto ribbon = dynamic_cast<RibbonParameter *>(eb->findParameterByID(r.id, VoiceGroup::I));
    nltools::test::_assert(ribbon != nullptr, "Parameter not Ribbon!");
    compareRibbon(ribbon, r);
  }

  for(auto &p : s.pedals)
  {
    auto pedal = dynamic_cast<PedalParameter *>(eb->findParameterByID(p.id));
    nltools::test::_assert(pedal != nullptr, "Parameter not Pedal!");
    comparePedal(pedal, p);
  }

  for(auto &m : s.master)
  {
    auto master = eb->findParameterByID(m.id, VoiceGroup::Global);
    nltools::test::_assert(master != nullptr, "Master not found!");
    compareUnmodulateable(master, m);
  }
}
