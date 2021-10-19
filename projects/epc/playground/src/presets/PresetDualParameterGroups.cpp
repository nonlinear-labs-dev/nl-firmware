#include <Application.h>
#include "EditBuffer.h"
#include "PresetDualParameterGroups.h"
#include "PresetManager.h"
#include "Preset.h"
#include <groups/ParameterGroup.h>
#include <presets/PresetParameterGroup.h>
#include <xml/Attribute.h>
#include <xml/Writer.h>
#include <sync/SyncMasterMockRoot.h>

PresetDualParameterGroups::PresetDualParameterGroups(UpdateDocumentContributor *parent)
    : AttributesOwner(parent)
{
  m_type = SoundType::Single;
  initEmpty();
}

PresetDualParameterGroups::PresetDualParameterGroups(UpdateDocumentContributor *parent, const Preset &other)
    : AttributesOwner(parent, &other)
{
  init(&other);
}

PresetDualParameterGroups::PresetDualParameterGroups(UpdateDocumentContributor *parent, const EditBuffer &editbuffer)
    : AttributesOwner(parent, &editbuffer)
{
  m_type = editbuffer.getType();

  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &g : editbuffer.getParameterGroups(vg))
      m_parameterGroups[static_cast<size_t>(vg)][g->getID()] = std::make_unique<PresetParameterGroup>(*g);
}

PresetDualParameterGroups::~PresetDualParameterGroups() = default;

void PresetDualParameterGroups::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &g : m_parameterGroups[static_cast<size_t>(vg)])
      g.second->writeDocument(writer);

  AttributesOwner::writeDocument(writer, knownRevision);
}

void PresetDualParameterGroups::copyFrom(UNDO::Transaction *transaction, const PresetDualParameterGroups *other)
{
  AttributesOwner::copyFrom(transaction, other);

  transaction->addUndoSwap(m_type, other->m_type);

  for(auto &vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    auto index = static_cast<size_t>(vg);
    for(auto &group : other->m_parameterGroups[index])
    {
      auto groupId = group.first;
      auto &myGroup = m_parameterGroups[index][groupId];
      myGroup->copyFrom(transaction, group.second.get());
    }
  }
}

void PresetDualParameterGroups::copyFrom(UNDO::Transaction *transaction, const EditBuffer *other)
{
  AttributesOwner::copyFrom(transaction, other);

  transaction->addUndoSwap(m_type, other->getType());

  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto g : other->getParameterGroups(vg))
      m_parameterGroups[static_cast<size_t>(vg)][g->getID()] = std::make_unique<PresetParameterGroup>(*g);
}

void PresetDualParameterGroups::copyVoiceGroup1IntoVoiceGroup2(UNDO::Transaction *transaction,
                                                               std::optional<std::set<GroupId>> whiteList)
{
  auto vgI = static_cast<size_t>(VoiceGroup::I);
  auto vgII = static_cast<size_t>(VoiceGroup::II);

  for(const auto &g : m_parameterGroups[vgI])
  {
    if(!whiteList || whiteList.value().count(g.first))
    {
      auto ptr = g.second.get();
      GroupId id { g.first.getName(), VoiceGroup::II };
      m_parameterGroups[vgII][id] = std::make_unique<PresetParameterGroup>(*ptr);
      m_parameterGroups[vgII][id]->assignVoiceGroup(transaction, VoiceGroup::II);
    }
  }
}

void PresetDualParameterGroups::writeGroups(Writer &writer, const Preset *other, VoiceGroup vgOfThis,
                                            VoiceGroup vgOfOther) const
{
  std::vector<std::string> writtenGroups;

  static std::vector<std::string> parameterGroupsThatAreTreatedAsGlobalForLayerSounds = { "Unison", "Mono" };

  auto isParameterGroupPresentInVGII = [&](GroupId id) {
    auto &v = parameterGroupsThatAreTreatedAsGlobalForLayerSounds;
    auto it = std::find(v.begin(), v.end(), id.getName());
    return it != v.end();
  };

  for(auto &g : m_parameterGroups[static_cast<size_t>(vgOfThis)])
  {
    PresetParameterGroup *myGroup = nullptr;
    PresetParameterGroup *otherGroup = nullptr;

    if(getType() == SoundType::Layer && isParameterGroupPresentInVGII(g.first))
    {
      myGroup = findParameterGroup({ g.first.getName(), VoiceGroup::I });
    }
    else
    {
      myGroup = g.second.get();
    }

    if(other->getType() == SoundType::Layer && isParameterGroupPresentInVGII(g.first))
    {
      otherGroup = other->findParameterGroup({ g.first.getName(), VoiceGroup::I });
    }
    else
    {
      otherGroup = other->findParameterGroup({ g.first.getName(), vgOfOther });
    }

    myGroup->writeDiff(writer, g.first, otherGroup);
    writtenGroups.emplace_back(g.first.getName());
  }

  for(auto &g : other->getGroups(vgOfOther))
  {
    if(std::find(writtenGroups.begin(), writtenGroups.end(), g.first.getName()) == writtenGroups.end())
      writer.writeTag("group", Attribute("name", g.first.getName()), Attribute("afound", "false"),
                      Attribute("bfound", "true"), [&] {});
  }
}

void PresetDualParameterGroups::init(const Preset *preset)
{
  m_type = preset->getType();

  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &group : preset->m_parameterGroups[static_cast<size_t>(vg)])
      m_parameterGroups[static_cast<size_t>(vg)][group.first] = std::make_unique<PresetParameterGroup>(*group.second);
}

void PresetDualParameterGroups::initEmpty()
{
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
    for(auto &group : getDataScheme().getParameterGroups(vg))
      m_parameterGroups[static_cast<size_t>(vg)][group->getID()] = std::make_unique<PresetParameterGroup>(*group);
}

PresetDualParameterGroups::GroupsMap &PresetDualParameterGroups::getGroups(VoiceGroup vg)
{
  return m_parameterGroups[static_cast<size_t>(vg)];
}

SoundType PresetDualParameterGroups::getType() const
{
  return m_type;
}

void PresetDualParameterGroups::setType(UNDO::Transaction *transaction, SoundType type)
{
  transaction->addUndoSwap(this, m_type, type);
}

PresetParameter *PresetDualParameterGroups::findParameterByID(ParameterId id, bool throwIfMissing) const
{
  for(auto &g : m_parameterGroups[static_cast<size_t>(id.getVoiceGroup())])
    if(auto p = g.second->findParameterByID(id))
      return p;

  if(throwIfMissing)
    throw std::runtime_error("no such parameter" + id.toString() + " in " + toString(id.getVoiceGroup()));

  return nullptr;
}

PresetParameterGroup *PresetDualParameterGroups::findParameterGroup(const GroupId &id) const
{
  const auto &groups = m_parameterGroups.at(static_cast<size_t>(id.getVoiceGroup()));
  auto it = groups.find(id);
  if(it != groups.end())
    return it->second.get();

  return nullptr;
}

void PresetDualParameterGroups::forEachParameter(const std::function<void(PresetParameter *)> &cb)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &g : m_parameterGroups[static_cast<size_t>(vg)])
      for(auto &p : g.second->getParameters())
        cb(p.get());
}

void PresetDualParameterGroups::forEachParameter(const std::function<void(const PresetParameter *)> &cb) const
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
    for(auto &g : m_parameterGroups[static_cast<size_t>(vg)])
      for(auto &p : g.second->getParameters())
        cb(p.get());
}

PresetParameterGroup *PresetDualParameterGroups::findOrCreateParameterGroup(const GroupId &id)
{
  if(auto ret = findParameterGroup(id))
  {
    return ret;
  }
  else if(auto schemeGroup = getDataScheme().getParameterGroupByID(id))
  {
    auto &vgMap = m_parameterGroups[static_cast<size_t>(id.getVoiceGroup())];
    vgMap[id] = std::make_unique<PresetParameterGroup>(*schemeGroup);
    return findParameterGroup(id);
  }
  std::stringstream ss;
  ss << "Could not find Parameter in Preset nor EditBuffer. ID: " << id.toString();
  nltools_detailedAssertAlways(false, ss.str());
}

PresetDualParameterGroups::tParameterGroups PresetDualParameterGroups::getGroups(const VoiceGroup &vg) const
{
  std::vector<std::pair<GroupId, const PresetParameterGroup *>> ret;
  for(const auto &g : m_parameterGroups[static_cast<size_t>(vg)])
  {
    ret.emplace_back(std::make_pair(g.first, g.second.get()));
  }
  return ret;
}

ParameterGroupSet &PresetDualParameterGroups::getDataScheme()
{
  static ParameterGroupSet sDataScheme(&SyncMasterMockRoot::get());
  sDataScheme.init(std::nullopt);
  return sDataScheme;
}
