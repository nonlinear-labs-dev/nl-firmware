#include "ParameterGroup.h"
#include "xml/Writer.h"
#include "parameters/Parameter.h"
#include "presets/ParameterGroupSet.h"
#include "presets/PresetParameterGroup.h"
#include <fstream>
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>
#include <nltools/logging/Log.h>
#include <libundo/undo/Scope.h>
#include <xml/Attribute.h>
#include <nltools/logging/Log.h>
#include <http/UpdateDocumentMaster.h>
#include <sync/JsonAdlSerializers.h>

ParameterGroup::ParameterGroup(ParameterGroupSet *parent, GroupId id, const char *shortName, const char *longName,
                               const char *webUIName)
    : UpdateDocumentContributor(parent)
    , SyncedItem(parent->getRoot()->getSyncMaster(), "/parametergroup/" + id.toString())
    , m_id(id)
    , m_shortName(shortName)
    , m_longName(longName)
    , m_webUIName(webUIName ? webUIName : m_longName)
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
                  Attribute("long-name", m_webUIName), Attribute("changed", changed),
                  [&]()
                  {
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

void ParameterGroup::validateParameterTypes() const
{
  bool hasAny = false;
  bool isPoly = false;
  bool isMono = false;
  bool isGlobal = false;
  bool isLocal = false;

  int polyCount = 0;
  int monoCount = 0;
  int globalCount = 0;
  int localCount = 0;

  for(const auto& p: getParameters())
  {
    auto foundType = p->getType();
    auto isPolyParam = foundType == C15::Descriptors::ParameterType::Polyphonic_Unmodulateable || foundType == C15::Descriptors::ParameterType::Polyphonic_Modulateable;
    auto isMonoParam = foundType == C15::Descriptors::ParameterType::Monophonic_Unmodulateable || foundType == C15::Descriptors::ParameterType::Monophonic_Modulateable;
    auto isGlobalParam = foundType == C15::Descriptors::ParameterType::Global_Modulateable || foundType == C15::Descriptors::ParameterType::Global_Unmodulateable;
    auto isLocalParam = foundType == C15::Descriptors::ParameterType::Local_Modulateable || foundType == C15::Descriptors::ParameterType::Local_Unmodulateable;

    polyCount += isPolyParam;
    monoCount += isMonoParam;
    globalCount += isGlobalParam;
    localCount += isLocalParam;

    if(!hasAny)
    {
      hasAny = true;
      isPoly = isPolyParam;
      isMono = isMonoParam;
      isGlobal = isGlobalParam;
      isLocal = isLocalParam;
    }
    else
    {
      nltools::Log::error("polyCount:", polyCount, "monoCount:", monoCount, "globalCount:", globalCount, "localCount:", localCount);

      nltools_detailedAssertAlways(isPoly == isPolyParam, "poly not consistent");
      nltools_detailedAssertAlways(isMono == isMonoParam, "mono not consistent");
      nltools_detailedAssertAlways(isGlobal == isGlobalParam, "global not consistent");
      nltools_detailedAssertAlways(isLocal == isLocalParam, "local not consistent");
    }
  }
}
