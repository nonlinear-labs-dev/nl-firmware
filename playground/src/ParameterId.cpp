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

  static std::array<int, 8> physicalContols{ 284, 289, 254, 259, 264, 269, 274, 279 };

  static std::array<int, 6> macroControls{ 243, 244, 245, 246, 369, 371 };
  static std::array<int, 6> macroTimes{ 324, 325, 326, 327, 370, 372 };

  static std::array<int, 48> macroRoutings{ 255, 256, 257, 258, 373, 374, 260, 261, 262, 263, 375, 376,
                                            265, 266, 267, 268, 377, 378, 270, 271, 272, 273, 379, 380,
                                            275, 276, 277, 278, 381, 382, 280, 281, 282, 283, 383, 384,
                                            285, 286, 287, 288, 385, 386, 290, 291, 292, 293, 387, 388 };

  if(std::find(physicalContols.begin(), physicalContols.end(), number) != physicalContols.end())
    return true;

  if(std::find(macroRoutings.begin(), macroRoutings.end(), number) != macroRoutings.end())
    return true;

  if(std::find(macroTimes.begin(), macroTimes.end(), number) != macroTimes.end())
    return true;

  if(std::find(macroControls.begin(), macroControls.end(), number) != macroControls.end())
    return true;

  if(number == 247 || number == 248)
    return true;

  return false;
}

ParameterId ParameterId::invalid()
{
  return ParameterId{ std::numeric_limits<decltype(m_num)>::max(), VoiceGroup::NumGroups };
}
