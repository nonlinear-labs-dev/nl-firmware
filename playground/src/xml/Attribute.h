#pragma once

#include "playground.h"
#include "ParameterId.h"
#include <glibmm.h>
#include <sstream>

namespace attribute_detail
{
  template <typename tValue> inline Glib::ustring toString(const tValue &v)
  {
    std::stringstream str;
    str << v;
    return str.str();
  }

  template <> inline Glib::ustring toString(const ParameterId &v)
  {
    return v.toString();
  }

  template <> inline Glib::ustring toString(const Glib::ustring &v)
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

  Glib::ustring toXmlString() const
  {
    return Glib::Markup::escape_text(m_name) + "=\"" + Glib::Markup::escape_text(m_value) + "\"";
  }

  const Glib::ustring &getName() const
  {
    return m_name;
  }

  const Glib::ustring &getValue() const
  {
    return m_value;
  }

 private:
  Glib::ustring m_name;
  Glib::ustring m_value;
};
