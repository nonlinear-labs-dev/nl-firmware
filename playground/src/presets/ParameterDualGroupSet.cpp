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

ParameterDualGroupSet::ParameterDualGroupSet(UpdateDocumentContributor *parent)
    : super(parent)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    appendParameterGroup(new EnvelopeAGroup(this), vg);
    appendParameterGroup(new EnvelopeBGroup(this), vg);
    appendParameterGroup(new EnvelopeCGroup(this), vg);
    appendParameterGroup(new OscillatorAGroup(this), vg);
    appendParameterGroup(new ShaperAGroup(this), vg);
    appendParameterGroup(new OscillatorBGroup(this), vg);
    appendParameterGroup(new ShaperBGroup(this), vg);
    appendParameterGroup(new FBMixerGroup(this), vg);
    appendParameterGroup(new CombFilterGroup(this), vg);
    appendParameterGroup(new SVFilterGroup(this), vg);
    appendParameterGroup(new OutputMixerGroup(this), vg);
    appendParameterGroup(new FlangerGroup(this), vg);
    appendParameterGroup(new CabinetGroup(this), vg);
    appendParameterGroup(new GapFilterGroup(this), vg);
    appendParameterGroup(new EchoGroup(this), vg);
    appendParameterGroup(new ReverbGroup(this), vg);
    appendParameterGroup(new MasterGroup(this), vg);
    appendParameterGroup(new UnisonGroup(this), vg);

    auto macroControls = appendParameterGroup(new MacroControlsGroup(this), vg);
    auto hwSources = appendParameterGroup(new HardwareSourcesGroup(this), vg);
    appendParameterGroup(new MacroControlMappingGroup(this, hwSources, macroControls), vg);
    appendParameterGroup(new ScaleGroup(this), vg);

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
    vg = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
  }

  for(auto a : m_parameterGroups[static_cast<int>(vg)])
    if(a->getID() == id)
      return a;

  return nullptr;
}

ParameterDualGroupSet::tParameterGroupPtr ParameterDualGroupSet::appendParameterGroup(ParameterGroup *p, VoiceGroup v)
{
  if(v == VoiceGroup::Invalid)
  {
    v = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
  }

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
}

Parameter *ParameterDualGroupSet::findParameterByID(int id, VoiceGroup vg) const
{
  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
  }

  try
  {
    return m_idToParameterMap[static_cast<int>(vg)].at(id);
  }
  catch(...)
  {
    return nullptr;
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
  if(vg == VoiceGroup::Invalid)
  {
    vg = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
  }

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
    vg = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
  }

  return m_parameterGroups[static_cast<int>(vg)];
}
