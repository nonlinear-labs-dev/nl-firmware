#pragma once

#include "playground.h"
#include "ParameterId.h"
#include "GroupId.h"

namespace attribute_detail
{
  template <typename tValue> inline Glib::ustring toString(const tValue &v)
  {
    return std::to_string(v);
  }

  template <> inline Glib::ustring toString(const ParameterId &v)
  {
    return v.toString();
  }


  template <> inline std::string toString(const GroupId &v)
  {
    return v.toString();
  }

  template <> inline std::string toString(const Glib::ustring &v)
  {
    return v;
  }
}

class Attribute
{
 public:
  template <typename tValue> inline Attribute(const Glib::ustring &name, const tValue &v)
  {
    m_name = name;
    m_value = attribute_detail::toString(v);
  }

  virtual ~Attribute();

  Glib::ustring toXmlString() const;
  const Glib::ustring &getName() const;
  const Glib::ustring &getValue() const;

 private:
  Glib::ustring m_name;
  Glib::ustring m_value;
};
