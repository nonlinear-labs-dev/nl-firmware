#include "GroupId.h"
#include <utility>
#include <glibmm/ustring.h>

static VoiceGroup parseGroup(const std::string &in)
{
  auto p = in.find('-');

  if(p == std::string::npos)
    return GroupId::isGlobal(in) ? VoiceGroup::Global : VoiceGroup::I;

  return to<VoiceGroup>(in.substr(0, p));
}

static std::string parseName(const std::string &in)
{
  auto p = in.find('-');

  if(p == std::string::npos)
    return in;

  return in.substr(p + 1);
}

GroupId::GroupId(std::string name, VoiceGroup group)
    : m_name(std::move(name))
    , m_group(group)
{
}

GroupId::GroupId(const GroupId &other)
    : m_name(other.m_name)
    , m_group(other.m_group)
{
}

GroupId::GroupId(const std::string &str)
    : m_name(parseName(str))
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

const std::string &GroupId::getName() const
{
  return m_name;
}

VoiceGroup GroupId::getVoiceGroup() const
{
  return m_group;
}

bool GroupId::isGlobal(const std::string &name)
{
  static std::vector<std::string> globals = { "CS", "Scale", "Master", "MCs", "MCM" };
  return std::any_of(globals.begin(), globals.end(), [name](auto a) { return a == name; });
}
