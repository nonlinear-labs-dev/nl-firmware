#include "ParameterGroup.h"
#include "xml/Writer.h"
#include "parameters/Parameter.h"
#include "presets/ParameterGroupSet.h"
#include "presets/PresetParameterGroup.h"
#include "parameter_group.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_list.h"
#include "parameters/ParameterFactory.h"
#include "parameters/UnisonVoicesParameter.h"
#include "parameters/SplitPointParameter.h"
#include "ScaleGroup.h"
#include "parameters/ScaleParameter.h"
#include <libundo/undo/Scope.h>
#include <xml/Attribute.h>
#include <nltools/logging/Log.h>
#include <http/UpdateDocumentMaster.h>
#include <sync/JsonAdlSerializers.h>

namespace
{
  GroupId createId(const C15::ParameterGroupDescriptor &desc, VoiceGroup vg)
  {
    return { desc.m_label_java, vg };
  }
}

ParameterGroup::ParameterGroup(ParameterGroupSet *parent, C15::ParameterGroupDescriptor desc, VoiceGroup vg)
    : UpdateDocumentContributor(parent)
    , SyncedItem(parent->getRoot()->getSyncMaster(), "/parametergroup/" + createId(desc, vg).toString())
    , m_id(createId(desc, vg))
    , m_shortName(desc.m_label_short)
    , m_longName(desc.m_label_long)
    , m_webUIName(desc.m_label_long)
    , m_groupDescriptor(desc)
{
}

ParameterGroup::~ParameterGroup()
{
  m_parameters.deleteItems();
}

GroupId ParameterGroup::getID() const
{
  return m_id;
}

size_t ParameterGroup::getHash() const
{
  size_t hash = 0;

  for(const auto p : m_parameters)
    hash_combine(hash, p->getHash());

  return hash;
}

Glib::ustring ParameterGroup::getShortName() const
{
  return m_shortName;
}

Glib::ustring ParameterGroup::getLongName() const
{
  return m_longName;
}

ParameterGroup::tParameterPtr ParameterGroup::getParameterByID(const ParameterId &id) const
{
  for(auto a : m_parameters)
    if(a->getID() == id)
      return a;

  nltools::throwException("Parameter not found!");
  return nullptr;
}

ParameterGroup::tParameterPtr ParameterGroup::findParameterByID(const ParameterId &id) const
{
  return getParameterByID(id);
}

void ParameterGroup::forEachParameter(const std::function<void(Parameter *)> &cb)
{
  for(auto p : m_parameters)
    cb(p);
}

const IntrusiveList<ParameterGroup::tParameterPtr> &ParameterGroup::getParameters() const
{
  return m_parameters;
}

IntrusiveList<ParameterGroup::tParameterPtr> &ParameterGroup::getParameters()
{
  return m_parameters;
}

sigc::connection ParameterGroup::onGroupChanged(const sigc::slot<void> &slot)
{
  return m_signalGroupChanged.connectAndInit(slot);
}

VoiceGroup ParameterGroup::getVoiceGroup() const
{
  return m_id.getVoiceGroup();
}

ParameterGroup::tUpdateID ParameterGroup::onChange(uint64_t flags)
{
  auto ret = super::onChange(flags);
  m_signalGroupChanged.deferedSend();
  SyncedItem::setDirty();
  return ret;
}

void ParameterGroup::copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other)
{
  for(auto &myParameter : getParameters())
  {
    if(auto otherParameter = other->findParameterByID({ myParameter->getID().getNumber(), other->getVoiceGroup() }))
    {
      myParameter->copyFrom(transaction, otherParameter);
    }
    else
    {
      myParameter->loadDefault(transaction, Defaults::FactoryDefault);
    }
  }
}

void ParameterGroup::undoableSetDefaultValues(UNDO::Transaction *transaction, const PresetParameterGroup *other)
{
  for(auto &g : getParameters())
  {
    PresetParameter *p = other ? other->findParameterByID(g->getID()) : nullptr;
    g->undoableSetDefaultValue(transaction, p);
  }
}

void ParameterGroup::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  bool changed = knownRevision < getUpdateIDOfLastChange();

  writer.writeTag("parameter-group", Attribute("id", getID()), Attribute("short-name", getShortName()),
                  Attribute("long-name", m_webUIName), Attribute("changed", changed), [&]() {
                    if(changed)
                      for(const auto p : m_parameters)
                        p->writeDocument(writer, knownRevision);
                  });
}

void ParameterGroup::undoableReset(UNDO::Transaction *transaction, Initiator initiator)
{
  for(auto p : getParameters())
    p->reset(transaction, initiator);
}

void ParameterGroup::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
  for(auto p : getParameters())
  {
    if(!p->isLocked() && !p->isDisabled())
    {
      p->undoableRandomize(transaction, initiator, amount);
    }
  }
}

void ParameterGroup::undoableLock(UNDO::Transaction *transaction)
{
  for(auto p : getParameters())
    p->undoableLock(transaction);
}

void ParameterGroup::undoableUnlock(UNDO::Transaction *transaction)
{
  for(auto p : getParameters())
    p->undoableUnlock(transaction);
}

void ParameterGroup::undoableToggleLock(UNDO::Transaction *transaction)
{
  if(areAllParametersLocked())
    undoableUnlock(transaction);
  else
    undoableLock(transaction);
}

bool ParameterGroup::isAnyParameterLocked() const
{
  for(auto p : getParameters())
    if(p->isLocked())
      return true;

  return false;
}

bool ParameterGroup::isAnyParameterChanged() const
{
  for(auto p : getParameters())
    if(p->isChangedFromLoaded())
      return true;

  return false;
}

bool ParameterGroup::areAllParametersLocked() const
{
  for(auto p : getParameters())
    if(!p->isLocked())
      return false;

  return true;
}

void ParameterGroup::copyFrom(UNDO::Transaction *transaction, const ParameterGroup *other)
{
  for(auto &myParameter : getParameters())
  {
    if(auto c = other->findParameterByID({ myParameter->getID().getNumber(), other->getVoiceGroup() }))
    {
      myParameter->copyFrom(transaction, c);
    }
    else
    {
      myParameter->loadDefault(transaction, Defaults::FactoryDefault);
    }
  }
}

void ParameterGroup::undoableLoadDefault(UNDO::Transaction *transaction, Defaults mode)
{
  for(auto &p : getParameters())
    p->loadDefault(transaction, mode);
}

namespace nlohmann
{
  template <> struct adl_serializer<Parameter *>
  {
    static void to_json(json &j, const Parameter *v)
    {
      j = v->getID();
    }
  };
}

nlohmann::json ParameterGroup::serialize() const
{
  return { { "id", getID() }, { "name", getLongName() }, { "parameters", m_parameters } };
}

bool ParameterGroup::isPolyphonic() const
{
  return m_parameters.begin()->isPolyphonic();
}

void ParameterGroup::init()
{
  for(auto id : ParameterFactory::getParameterIDs(m_groupDescriptor.m_group))
  {
    appendParameter(ParameterFactory::createParameterByType(this, { id, getVoiceGroup() }));
  }
}
