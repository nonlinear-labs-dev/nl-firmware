#include "ParameterGroupSet.h"
#include <presets/Preset.h>

#include "parameters/Parameter.h"
#include "groups/MacroControlsGroup.h"
#include "groups/MasterGroup.h"
#include "groups/HardwareSourcesGroup.h"
#include <groups/MacroControlMappingGroup.h>
#include <groups/ScaleGroup.h>

#include "xml/Writer.h"
#include "xml/Attribute.h"

#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <groups/VoiceGroupMasterGroup.h>
#include <parameter_declarations.h>

ParameterGroupSet::ParameterGroupSet(UpdateDocumentContributor *parent)
    : super(parent)
{
}

void ParameterGroupSet::init(Settings *settings)
{
  auto hwSources = appendParameterGroup(new HardwareSourcesGroup(this, settings));
  auto macroControls = appendParameterGroup(new MacroControlsGroup(this));
  appendParameterGroup(new MacroControlMappingGroup(this, hwSources, macroControls));

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    appendParameterGroup(new ParameterGroup(this, { "Env A", vg }, "Envelope A", "Envelope A", "Envelope A"));
    appendParameterGroup(new ParameterGroup(this, { "Env B", vg }, "Envelope B", "Envelope B", "Envelope B"));
    appendParameterGroup(new ParameterGroup(this, { "Env C", vg }, "Envelope C", "Envelope C", "Envelope C"));
    appendParameterGroup(new ParameterGroup(this, { "Osc A", vg }, "Oscillator A", "Oscillator A", "Oscillator A"));
    appendParameterGroup(new ParameterGroup(this, { "Sh A", vg }, "Shaper A", "Shaper A", "Shaper A"));
    appendParameterGroup(new ParameterGroup(this, { "Osc B", vg }, "Oscillator B", "Oscillator B", "Oscillator B"));
    appendParameterGroup(new ParameterGroup(this, { "Sh B", vg }, "Shaper B", "Shaper B", "Shaper B"));
    appendParameterGroup(new ParameterGroup(this, { "FB", vg }, "FB Mixer", "Feedback Mixer", "Feedback Mixer"));
    appendParameterGroup(new ParameterGroup(this, { "Comb", vg }, "Comb Filter", "Comb Filter", "Comb Filter"));
    appendParameterGroup(
        new ParameterGroup(this, { "SVF", vg }, "SV Filter", "State Variable Filter", "State Variable Filter"));
    appendParameterGroup(new ParameterGroup(this, { "Mixer", vg }, "Output Mixer", "Output Mixer", "Output Mixer"));
    appendParameterGroup(new ParameterGroup(this, { "Flang", vg }, "Flanger", "Flanger", "Flanger"));
    appendParameterGroup(new ParameterGroup(this, { "Cab", vg }, "Cabinet", "Cabinet", "Cabinet"));
    appendParameterGroup(new ParameterGroup(this, { "Gap Filt", vg }, "Gap Filter", "Gap Filter", "Gap Filter"));
    appendParameterGroup(new ParameterGroup(this, { "Echo", vg }, "Echo", "Echo", "Echo"));
    appendParameterGroup(new ParameterGroup(this, { "Reverb", vg }, "Reverb", "Reverb", "Reverb"));
    appendParameterGroup(new ParameterGroup(this, { "Unison", vg }, "Unison", "Unison", "Unison"));
    appendParameterGroup(new ParameterGroup(this, { "Split", vg }, "Split", "Split", "Split"));
    appendParameterGroup(new ParameterGroup(this, { "Mono", vg }, "Mono", "Mono", "Mono"));
    appendParameterGroup(new VoiceGroupMasterGroup(this, vg));

    m_idToParameterMap[static_cast<size_t>(vg)] = getParametersSortedByNumber(vg);
  }

  appendParameterGroup(new MasterGroup(this));
  appendParameterGroup(new ScaleGroup(this));

  m_idToParameterMap[static_cast<size_t>(VoiceGroup::Global)] = getParametersSortedByNumber(VoiceGroup::Global);
}

ParameterGroupSet::~ParameterGroupSet()
{
  for(auto &i : m_parameterGroups)
    i.deleteItems();
}

ParameterGroupSet::tParameterGroupPtr ParameterGroupSet::getParameterGroupByID(const GroupId &id) const
{
  for(auto a : m_parameterGroups[static_cast<size_t>(id.getVoiceGroup())])
    if(a->getID().getName() == id.getName())
      return a;

  return nullptr;
}

ParameterGroupSet::tParameterGroupPtr ParameterGroupSet::appendParameterGroup(ParameterGroup *p)
{
  p->init();
  auto id = p->getID();
  nltools_assertAlways(getParameterGroupByID(id) == nullptr);
  auto wrapped = tParameterGroupPtr(p);
  m_parameterGroups[static_cast<size_t>(p->getID().getVoiceGroup())].append(wrapped);
  return wrapped;
}

void ParameterGroupSet::copyFrom(UNDO::Transaction *transaction, const Preset *other)
{
  super::copyFrom(transaction, other);

  setAttribute(transaction, "origin-I", other->getUuid().raw());
  setAttribute(transaction, "origin-II", other->getUuid().raw());

  if(other->isDual())
  {
    setAttribute(transaction, "origin-I-vg", toString(VoiceGroup::I));
    setAttribute(transaction, "origin-II-vg", toString(VoiceGroup::II));
  }
  else
  {
    setAttribute(transaction, "origin-I-vg", toString(VoiceGroup::I));
    setAttribute(transaction, "origin-II-vg", toString(VoiceGroup::I));
  }

  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &g : getParameterGroups(vg))
      if(auto c = other->findParameterGroup(g->getID()))
        g->copyFrom(transaction, c);
      else
        g->forEachParameter([&](Parameter *p) { p->loadDefault(transaction, Defaults::FactoryDefault); });

  copyGlobalSplitIntoDualSplit(transaction, other);
}

void ParameterGroupSet::copyGlobalSplitIntoDualSplit(UNDO::Transaction *transaction, const Preset *other)
{
  if(other->getType() == SoundType::Split)
  {
    if(auto presetSplit = other->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::Global }, false))
    {
      for(auto vg : { VoiceGroup::I, VoiceGroup::II })
      {
        if(auto split = findParameterByID({ C15::PID::Split_Split_Point, vg }))
        {
          split->copyFrom(transaction, presetSplit);
        }
      }
    }
  }
}

Parameter *ParameterGroupSet::findParameterByID(const ParameterId &id) const
{
  if(static_cast<size_t>(id.getVoiceGroup()) >= m_idToParameterMap.size())
    return nullptr;

  auto &m = m_idToParameterMap.at(static_cast<size_t>(id.getVoiceGroup()));
  auto it = m.find(id.getNumber());

  if(it == m.end())
    return nullptr;

  return it->second;
}

void ParameterGroupSet::forEachParameter(VoiceGroup vg, const std::function<void(Parameter *)> &cb)
{
  for(auto g : getParameterGroups(vg))
    for(auto p : g->getParameters())
      cb(p);
}

void ParameterGroupSet::forEachParameter(const std::function<void(Parameter *)> &cb)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto g : getParameterGroups(vg))
      for(auto p : g->getParameters())
        cb(p);
}

void ParameterGroupSet::forEachParameter(const std::function<void(const Parameter *)> &cb) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto g : getParameterGroups(vg))
      for(auto p : g->getParameters())
        cb(p);
}

std::map<int, Parameter *> ParameterGroupSet::getParametersSortedByNumber(VoiceGroup vg) const
{
  std::map<int, Parameter *> sorted;

  for(auto group : getParameterGroups(vg))
    for(auto param : group->getParameters())
      sorted[param->getID().getNumber()] = param;

  return sorted;
}

void ParameterGroupSet::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  super::writeDocument(writer, knownRevision);

  auto writePerVoiceGroup = [&](auto id, auto tag) {
    auto &groups = getParameterGroups(id);
    auto anyGroupChanged = false;

    for(auto &p : groups)
      anyGroupChanged |= p->didChangeSince(knownRevision);

    writer.writeTag(tag, Attribute("changed", anyGroupChanged), [&] {
      if(anyGroupChanged)
        for(auto &p : groups)
          p->writeDocument(writer, knownRevision);
    });
  };

  writePerVoiceGroup(VoiceGroup::Global, "global-parameters");
  writePerVoiceGroup(VoiceGroup::I, "voice-group-I-parameters");
  writePerVoiceGroup(VoiceGroup::II, "voice-group-II-parameters");
}

const IntrusiveList<ParameterGroupSet::tParameterGroupPtr> &ParameterGroupSet::getParameterGroups(VoiceGroup vg) const
{
  return m_parameterGroups.at(static_cast<size_t>(vg));
}

void ParameterGroupSet::copyFrom(UNDO::Transaction *transaction, const Preset *preset, VoiceGroup from, VoiceGroup to)
{
  setAttribute(transaction, to == VoiceGroup::I ? "origin-I" : "origin-II", preset->getUuid().raw());
  setAttribute(transaction, to == VoiceGroup::I ? "origin-I-vg" : "origin-II-vg", toString(from));

  for(auto myGroup : getParameterGroups(to))
  {
    if(auto other = preset->findParameterGroup({ myGroup->getID().getName(), from }))
    {
      myGroup->copyFrom(transaction, other);
    }
    else
    {
      myGroup->undoableLoadDefault(transaction, Defaults::FactoryDefault);
    }
  }
}

void ParameterGroupSet::copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to)
{
  for(auto &group : getParameterGroups(to))
    group->copyFrom(transaction, getParameterGroupByID({ group->getID().getName(), from }));
}

void ParameterGroupSet::loadSinglePresetIntoVoiceGroup(UNDO::Transaction *transaction, const Preset *p,
                                                       VoiceGroup target)
{
  nltools_assertOnDevPC(p->getType() == SoundType::Single);

  for(auto &g : getParameterGroups(target))
  {
    if(auto c = p->findParameterGroup({ g->getID().getName(), VoiceGroup::I }))
    {
      g->copyFrom(transaction, c);
    }
    else
    {
      g->undoableLoadDefault(transaction, Defaults::FactoryDefault);
    }
  }

  if(target == VoiceGroup::I)
  {
    setAttribute(transaction, "origin-I", p->getUuid().raw());
    setAttribute(transaction, "origin-I-vg", toString(VoiceGroup::I));
  }
  else if(target == VoiceGroup::II)
  {
    setAttribute(transaction, "origin-II", p->getUuid().raw());
    setAttribute(transaction, "origin-I-vg", toString(VoiceGroup::II));
  }
}

const ParameterGroupSet::tParamArray &ParameterGroupSet::getParameters() const
{
  return m_parameterGroups;
}