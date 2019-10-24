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
    auto hwSources = appendParameterGroup(new HardwareSourcesGroup(this, vg), vg);
    appendParameterGroup(new MacroControlMappingGroup(this, hwSources, macroControls, vg), vg);

    appendParameterGroup(new MonoGroup(this, vg), vg);
    appendParameterGroup(new VoiceGroupMasterGroup(this, vg), vg);

    m_idToParameterMap[static_cast<int>(vg)] = getParametersSortedById(vg);
  }

  appendGlobalParameterGroup(new GlobalParameterGroups(this));
  appendGlobalParameterGroup(new MasterGroup(this));
  appendGlobalParameterGroup(new ScaleGroup(this, VoiceGroup::Global));

  m_globalIDToParameterMap = getGlobalParametersSortedById();
}

ParameterDualGroupSet::~ParameterDualGroupSet()
{
  for(auto &i : m_polyParameterGroups)
    i.deleteItems();
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::getParameterGroupByID(const Glib::ustring &id,
                                                                                       VoiceGroup vg) const
{
  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  }

  for(auto a : m_polyParameterGroups[static_cast<int>(vg)])
    if(a->getID() == id)
      return a;


  for(auto a: m_globalParameterGroups)
    if(a->getID() == id)
      return a;

  return nullptr;
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::appendParameterGroup(ParameterGroup *p, VoiceGroup v)
{
  p->init();
  g_assert(getParameterGroupByID(p->getID(), v) == nullptr);
  auto wrapped = tParameterGroupPtr(p);
  m_polyParameterGroups[static_cast<int>(v)].append(wrapped);
  return wrapped;
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::appendGlobalParameterGroup(ParameterGroup *p)
{
  p->init();
  g_assert(getGlobalParameterGroupByID(p->getID()) == nullptr);
  auto wrapped = tParameterGroupPtr(p);
  m_globalParameterGroups.append(wrapped);
  return wrapped;
}

void ParameterDualGroupSet::copyFrom(UNDO::Transaction *transaction, const Preset *other)
{

  super::copyFrom(transaction, other);

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto &g : getParameterGroups(vg))
    {
      if(auto c = other->findParameterGroup(g->getID(), vg))
      {
        g->copyFrom(transaction, c);
      }
    }
  }

  for(auto &g : m_globalParameterGroups)
  {
    if(auto o = other->findParameterGroup(g->getID(), VoiceGroup::Global))
      g->copyFrom(transaction, o);
  }
}

Parameter *ParameterDualGroupSet::findParameterByID(int id, VoiceGroup vg) const
{
  if(vg == VoiceGroup::Global)
  {
    return findGlobalParameterByID(id);
  }

  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  }

  try
  {
    auto ret = m_idToParameterMap[static_cast<int>(vg)].at(id);
    nltools_assertOnDevPC(ret->getVoiceGroup() == VoiceGroup::I || ret->getVoiceGroup() == VoiceGroup::II);
    return ret;
  }
  catch(...)
  {
    return nullptr;
  }
}

Parameter *ParameterDualGroupSet::findGlobalParameterByID(int id) const
{
  try
  {
    auto ret = m_globalIDToParameterMap.at(id);
    nltools_assertOnDevPC(ret->getVoiceGroup() == VoiceGroup::Global);
    return ret;
  }
  catch(...)
  {
    return nullptr;
  }
}

size_t ParameterDualGroupSet::countParameters() const
{
  size_t count = 0;

  for(auto group : m_polyParameterGroups[0])
    count += group->countParameters();

  return count;
}

std::map<int, Parameter *> ParameterDualGroupSet::getParametersSortedById(VoiceGroup vg) const
{
  std::map<int, Parameter *> sorted;

  for(auto group : getParameterGroups(vg))
    for(auto param : group->getParameters())
      sorted[param->getID()] = param;

  return sorted;
}

std::map<int, Parameter *> ParameterDualGroupSet::getGlobalParametersSortedById() const
{
  std::map<int, Parameter *> sorted;

  for(auto g : getGlobalParameterGroups())
    for(auto p : g->getParameters())
      sorted[p->getID()] = p;

  return sorted;
}

void ParameterDualGroupSet::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
#warning "TODO add webui focus!!! see and do @PresetDualParameterGroups"
  super::writeDocument(writer, knownRevision);

  for(tParameterGroupPtr p : getParameterGroups(VoiceGroup::I))
    p->writeDocument(writer, knownRevision);
}

const IntrusiveList<ParameterDualGroupSet::tParameterGroupPtr> &
    ParameterDualGroupSet::getParameterGroups(VoiceGroup vg) const
{
  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  }

  return m_polyParameterGroups[static_cast<int>(vg)];
}

const IntrusiveList<ParameterDualGroupSet::tParameterGroupPtr> &ParameterDualGroupSet::getGlobalParameterGroups() const
{
  return m_globalParameterGroups;
}

void ParameterDualGroupSet::copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to)
{
  nltools_assertOnDevPC(from != to && to != VoiceGroup::Invalid && from != VoiceGroup::Invalid);

  for(auto &group : getParameterGroups(to))
  {
    group->copyFrom(transaction, getParameterGroupByID(group->getID(), from));
  }
}

void ParameterDualGroupSet::loadIntoVoiceGroup(UNDO::Transaction *transaction, Preset *p, VoiceGroup target)
{
  nltools_assertOnDevPC(p->getType() == SoundType::Single);

  super::copyFrom(transaction, p);

  for(auto &g : getParameterGroups(target))
  {
    if(auto c = p->findParameterGroup(g->getID(), VoiceGroup::I))
    {
      g->copyFrom(transaction, c);
    }
  }
}

ParameterDualGroupSet::tParameterGroupPtr
    ParameterDualGroupSet::getGlobalParameterGroupByID(const Glib::ustring &id) const
{
  for(auto a : m_globalParameterGroups)
    if(a->getID() == id)
      return a;

  return nullptr;
}
