#pragma once

#include <nltools/Types.h>

class GroupId
{
 public:
  GroupId(const std::string &name, VoiceGroup group);
  GroupId(const GroupId &other);
  GroupId(const std::string &str);
  GroupId(const Glib::ustring &str);

  bool operator<(const GroupId &other) const;
  bool operator==(const GroupId &other) const;
  bool operator!=(const GroupId &other) const;

  std::string getName() const;
  VoiceGroup getVoiceGroup() const;

  std::string toString() const
  {
    return ::toString(getVoiceGroup()) + "-" + getName();
  }

 private:
  std::string m_name;
  VoiceGroup m_group;
};

std::ostream &operator<<(std::ostream &stream, const GroupId &e)
{
  stream << e.toString();
  return stream;
}
