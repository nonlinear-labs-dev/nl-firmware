#pragma once

#include <nltools/Types.h>

class ParameterId
{
 public:
  ParameterId(int num, VoiceGroup group);
  ParameterId(const ParameterId &other);
  ParameterId(const std::string &str);
  ParameterId(const Glib::ustring &str);

  bool operator<(const ParameterId &other) const;
  bool operator==(const ParameterId &other) const;
  bool operator!=(const ParameterId &other) const;

  int getNumber() const;
  VoiceGroup getVoiceGroup() const;

  std::string toString() const
  {
    return ::toString(getVoiceGroup()) + "-" + std::to_string(getNumber());
  }

 private:
  int m_num;
  VoiceGroup m_group;
};

std::ostream &operator<<(std::ostream &stream, const ParameterId &e)
{
  stream << e.toString();
  return stream;
}
