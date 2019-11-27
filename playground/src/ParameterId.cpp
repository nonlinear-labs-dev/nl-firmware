#include "ParameterId.h"
#include <utility>

static int parseNumber(const std::string &in)
{
  auto p = in.find('-');
  return std::atoi(in.data() + p + 1);
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
    : m_num(parseNumber(str))
    , m_group(parseGroup(str))
{
}

ParameterId::ParameterId(const Glib::ustring &str)
    : ParameterId(str.raw())
{
}

bool ParameterId::operator<(const ParameterId &other) const
{
  return std::tie(m_num, m_group) < std::tie(other.m_num, other.m_group);
}

bool ParameterId::operator==(const ParameterId &other) const
{
  return std::tie(m_num, m_group) == std::tie(other.m_num, other.m_group);
}

bool ParameterId::operator!=(const ParameterId &other) const
{
  return std::tie(m_num, m_group) != std::tie(other.m_num, other.m_group);
}

int ParameterId::getNumber() const
{
  return m_num;
}

VoiceGroup ParameterId::getVoiceGroup() const
{
  return m_group;
}
