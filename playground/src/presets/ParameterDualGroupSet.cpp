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
#include <groups/SplitSoundGroup.h>

ParameterDualGroupSet::ParameterDualGroupSet(UpdateDocumentContributor *parent)
    : super(parent)
    , m_splitParameters{ this }
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
    appendParameterGroup(new MasterGroup(this, vg), vg);
    appendParameterGroup(new UnisonGroup(this, vg), vg);

    auto macroControls = appendParameterGroup(new MacroControlsGroup(this, vg), vg);
    auto hwSources = appendParameterGroup(new HardwareSourcesGroup(this, vg), vg);
    appendParameterGroup(new MacroControlMappingGroup(this, hwSources, macroControls, vg), vg);
    appendParameterGroup(new ScaleGroup(this, vg), vg);

    m_idToParameterMap[static_cast<int>(vg)] = getParametersSortedById(vg);
  }
}

ParameterDualGroupSet::~ParameterDualGroupSet()
{
  for(auto &i : m_parameterGroups)
    i.deleteItems();
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::getParameterGroupByID(const Glib::ustring &id,
                                                                                       VoiceGroup vg) const
{
  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  }

  for(auto a : m_parameterGroups[static_cast<int>(vg)])
    if(a->getID() == id)
      return a;

  return nullptr;
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::appendParameterGroup(ParameterGroup *p, VoiceGroup v)
{
  p->init();
  g_assert(getParameterGroupByID(p->getID(), v) == nullptr);
  auto wrapped = tParameterGroupPtr(p);
  m_parameterGroups[static_cast<int>(v)].append(wrapped);
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

  if(other->getSplitGroup())
    m_splitParameters.copyFrom(transaction, other->getSplitGroup());
}

Parameter *ParameterDualGroupSet::findParameterByID(int id, VoiceGroup vg) const
{
  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  }

  try
  {
    return m_idToParameterMap[static_cast<int>(vg)].at(id);
  }
  catch(...)
  {
    try
    {
      return m_splitParameters.findParameterByID(id);
    }
    catch(...)
    {
      return nullptr;
    }
  }
}

size_t ParameterDualGroupSet::countParameters() const
{
  size_t count = 0;

  for(auto group : m_parameterGroups[0])
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

  return m_parameterGroups[static_cast<int>(vg)];
}

void ParameterDualGroupSet::copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to)
{
  nltools_assertOnDevPC(from != VoiceGroup::Global && to != VoiceGroup::Global);
  nltools_assertOnDevPC(from != to && to != VoiceGroup::Invalid && from != VoiceGroup::Invalid);

  for(auto &group : getParameterGroups(to))
  {
    group->copyFrom(transaction, getParameterGroupByID(group->getID(), from));
  }
}

SplitSoundGroup *ParameterDualGroupSet::getSplitSoundParameterGroup()
{
  return &m_splitParameters;
}

const SplitSoundGroup *ParameterDualGroupSet::getSplitSoundParameterGroup() const
{
  return &m_splitParameters;
}