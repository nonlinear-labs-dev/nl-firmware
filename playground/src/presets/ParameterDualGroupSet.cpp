#include "ParameterDualGroupSet.h"
#include <presets/Preset.h>

#include "../groups/GapFilterGroup.h"
#include "parameters/Parameter.h"

#include "groups/EnvelopeAGroup.h"
#include "groups/EnvelopeBGroup.h"
#include "groups/EnvelopeCGroup.h"
#include "groups/OscillatorAGroup.h"
#include "groups/ShaperAGroup.h"
#include "groups/OscillatorBGroup.h"
#include "groups/ShaperBGroup.h"
#include "groups/FBMixerGroup.h"
#include "groups/CombFilterGroup.h"
#include "groups/SVFilterGroup.h"
#include "groups/OutputMixerGroup.h"
#include "groups/CabinetGroup.h"
#include "groups/FlangerGroup.h"
#include "groups/EchoGroup.h"
#include "groups/ReverbGroup.h"
#include "groups/MacroControlsGroup.h"
#include "groups/MasterGroup.h"
#include "groups/UnisonGroup.h"
#include "groups/HardwareSourcesGroup.h"
#include <groups/MacroControlMappingGroup.h>
#include <groups/ScaleGroup.h>

#include "xml/Writer.h"

#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <groups/MonoGroup.h>
#include <groups/GlobalParameterGroups.h>
#include <groups/VoiceGroupMasterGroup.h>

ParameterDualGroupSet::ParameterDualGroupSet(UpdateDocumentContributor *parent)
    : super(parent)
{
  auto hwSources = appendParameterGroup(new HardwareSourcesGroup(this), VoiceGroup::Global);

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    appendParameterGroup(new EnvelopeAGroup(this, vg), vg);
    appendParameterGroup(new EnvelopeBGroup(this, vg), vg);
    appendParameterGroup(new EnvelopeCGroup(this, vg), vg);
    appendParameterGroup(new OscillatorAGroup(this, vg), vg);
    appendParameterGroup(new ShaperAGroup(this, vg), vg);
    appendParameterGroup(new OscillatorBGroup(this, vg), vg);
    appendParameterGroup(new ShaperBGroup(this, vg), vg);
    appendParameterGroup(new FBMixerGroup(this, vg), vg);
    appendParameterGroup(new CombFilterGroup(this, vg), vg);
    appendParameterGroup(new SVFilterGroup(this, vg), vg);
    appendParameterGroup(new OutputMixerGroup(this, vg), vg);
    appendParameterGroup(new FlangerGroup(this, vg), vg);
    appendParameterGroup(new CabinetGroup(this, vg), vg);
    appendParameterGroup(new GapFilterGroup(this, vg), vg);
    appendParameterGroup(new EchoGroup(this, vg), vg);
    appendParameterGroup(new ReverbGroup(this, vg), vg);
    appendParameterGroup(new UnisonGroup(this, vg), vg);

    auto macroControls = appendParameterGroup(new MacroControlsGroup(this, vg), vg);
    appendParameterGroup(new MacroControlMappingGroup(this, hwSources, macroControls, vg), vg);

    appendParameterGroup(new MonoGroup(this, vg), vg);
    appendParameterGroup(new VoiceGroupMasterGroup(this, vg), vg);

    m_idToParameterMap[static_cast<size_t>(vg)] = getParametersSortedByNumber(vg);
  }

  appendParameterGroup(new GlobalParameterGroups(this), VoiceGroup::Global);
  appendParameterGroup(new MasterGroup(this), VoiceGroup::Global);
  appendParameterGroup(new ScaleGroup(this), VoiceGroup::Global);

  m_idToParameterMap[static_cast<size_t>(VoiceGroup::Global)] = getParametersSortedByNumber(VoiceGroup::Global);
}

ParameterDualGroupSet::~ParameterDualGroupSet()
{
  for(auto &i : m_parameterGroups)
    i.deleteItems();
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::getParameterGroupByID(const Glib::ustring &id,
                                                                                       VoiceGroup vg) const
{
  for(auto a : m_parameterGroups[static_cast<size_t>(vg)])
    if(a->getID() == id)
      return a;

  return nullptr;
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::appendParameterGroup(ParameterGroup *p, VoiceGroup v)
{
  p->init();
  g_assert(getParameterGroupByID(p->getID(), v) == nullptr);
  auto wrapped = tParameterGroupPtr(p);
  m_parameterGroups[static_cast<size_t>(v)].append(wrapped);
  return wrapped;
}

void ParameterDualGroupSet::copyFrom(UNDO::Transaction *transaction, const Preset *other)
{
  super::copyFrom(transaction, other);

  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &g : getParameterGroups(vg))
      if(auto c = other->findParameterGroup(g->getID(), vg))
        g->copyFrom(transaction, c);
}

Parameter *ParameterDualGroupSet::findParameterByID(ParameterId id) const
{
  try
  {
    return m_idToParameterMap.at(static_cast<size_t>(id.getVoiceGroup())).at(id.getNumber());
  }
  catch(...)
  {
    return nullptr;
  }
}

std::map<int, Parameter *> ParameterDualGroupSet::getParametersSortedByNumber(VoiceGroup vg) const
{
  std::map<int, Parameter *> sorted;

  for(auto group : getParameterGroups(vg))
    for(auto param : group->getParameters())
      sorted[param->getID().getNumber()] = param;

  return sorted;
}

void ParameterDualGroupSet::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  super::writeDocument(writer, knownRevision);

  writer.writeTag("global-parameters", [&] {
    for(tParameterGroupPtr p : getParameterGroups(VoiceGroup::Global))
      p->writeDocument(writer, knownRevision);
  });

  writer.writeTag("voice-group-I-parameters", [&] {
    for(tParameterGroupPtr p : getParameterGroups(VoiceGroup::I))
      p->writeDocument(writer, knownRevision);
  });

  writer.writeTag("voice-group-II-parameters", [&] {
    for(tParameterGroupPtr p : getParameterGroups(VoiceGroup::II))
      p->writeDocument(writer, knownRevision);
  });
}

const IntrusiveList<ParameterDualGroupSet::tParameterGroupPtr> &
    ParameterDualGroupSet::getParameterGroups(VoiceGroup vg) const
{
  try
  {
    return m_parameterGroups.at(static_cast<size_t>(vg));
  }
  catch(...)
  {
    nltools::fail("Could not find Parameter Groups", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ParameterDualGroupSet::copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to)
{
  for(auto &group : getParameterGroups(to))
    group->copyFrom(transaction, getParameterGroupByID(group->getID(), from));
}

void ParameterDualGroupSet::loadIntoVoiceGroup(UNDO::Transaction *transaction, Preset *p, VoiceGroup target)
{
  nltools_assertOnDevPC(p->getType() == SoundType::Single);

  super::copyFrom(transaction, p);

  for(auto &g : getParameterGroups(target))
    if(auto c = p->findParameterGroup(g->getID(), VoiceGroup::I))
      g->copyFrom(transaction, c);

  for(auto &g : getParameterGroups(VoiceGroup::Global))
    for(auto &globalParam : g->getParameters())
      if(auto presetGlobalParam = p->findParameterByID(globalParam->getID()))
        globalParam->copyFrom(transaction, presetGlobalParam);
}
