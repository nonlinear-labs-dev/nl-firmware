#include "ParameterId.h"
#include <utility>

static int parseName(const std::string &in)
{
  auto p = in.find('-');
  return in.substr(0, p);
}

static VoiceGroup parseGroup(const std::string &in)
{
  auto p = in.find('-');
  return to<VoiceGroup>(in.substr(0, p));
}

ParameterId::ParameterId(int num, VoiceGroup group)
    : m_num(num)
    , m_group(group)
{
}

ParameterId::ParameterId(const ParameterId &other)
    : m_num(other.m_num)
    , m_group(other.m_group)
{
}

ParameterId::ParameterId(const std::string &str)
    : m_name(parseName(str))
    , m_group(parseGroup(str))
{
}

ParameterId::ParameterId(const Glib::ustring &str)
    : ParameterId(str.raw())
{
}

bool ParameterId::operator<(const ParameterId &other) const
{
  return std::tie(m_name, m_group) < std::tie(other.m_name, other.m_group);
}

bool ParameterId::operator==(const ParameterId &other) const
{
  return std::tie(m_name, m_group) == std::tie(other.m_name, other.m_group);
}

bool ParameterId::operator!=(const ParameterId &other) const
{
  return std::tie(m_name, m_group) != std::tie(other.m_name, other.m_group);
}

int ParameterId::getName() const
{
  return m_name;
}

VoiceGroup ParameterId::getVoiceGroup() const
{
  return m_group;
}
