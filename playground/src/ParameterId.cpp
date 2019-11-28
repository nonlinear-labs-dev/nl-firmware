#include "ParameterId.h"
#include <utility>

static int parseNumber(const std::string &in)
{
  auto p = in.find('-');

  if(p == std::string::npos)
    return std::atoi(in.data());

  return std::atoi(in.data() + p + 1);
}

static VoiceGroup parseGroup(const std::string &in)
{
  auto p = in.find('-');

  if(p == std::string::npos)
  {
    auto n = parseNumber(in);
    return ParameterId::isGlobal(n) ? VoiceGroup::Global : VoiceGroup::I;
  }

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

uint16_t ParameterId::getNumber() const
{
  return m_num;
}

VoiceGroup ParameterId::getVoiceGroup() const
{
  return m_group;
}

bool ParameterId::isGlobal(int number)
{
  if(number >= 312 && number <= 323)
    return true;

  if(number == 356)
    return true;

  std::array<int, 8> physicalContols{ 284, 289, 254, 259, 264, 269, 274, 279 };

  if(std::find(physicalContols.begin(), physicalContols.end(), number) != physicalContols.end())
    return true;

  if(number == 247 || number == 248)
    return true;

  return false;
}
