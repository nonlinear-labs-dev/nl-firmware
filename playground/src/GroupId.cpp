#include "GroupId.h"
#include "GroupId.h"

#include <utility>

static VoiceGroup parseGroup(const std::string &in)
{
  auto p = in.find('-');
  return to<VoiceGroup>(in.substr(0, p));
}

GroupId::GroupId(const GroupId &other)
    : m_name(other.m_name)
    , m_group(other.m_group)
{
}

GroupId::GroupId(const std::string &str)
    : m_name(str)
    , m_group(parseGroup(str))
{
}

GroupId::GroupId(const Glib::ustring &str)
    : GroupId(str.raw())
{
}

bool GroupId::operator<(const GroupId &other) const
{
  return std::tie(m_name, m_group) < std::tie(other.m_name, other.m_group);
}

bool GroupId::operator==(const GroupId &other) const
{
  return std::tie(m_name, m_group) == std::tie(other.m_name, other.m_group);
}

bool GroupId::operator!=(const GroupId &other) const
{
  return std::tie(m_name, m_group) != std::tie(other.m_name, other.m_group);
}

const std::string& GroupId::getName() const
{
  return m_name;
}

VoiceGroup GroupId::getVoiceGroup() const
{
  return m_group;
}