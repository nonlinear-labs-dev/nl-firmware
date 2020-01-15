#include "Attribute.h"
#include <ParameterId.h>
#include <GroupId.h>
#include <glibmm.h>

Attribute::~Attribute() = default;

Glib::ustring Attribute::toXmlString() const
{
  return Glib::Markup::escape_text(m_name) + "=\"" + Glib::Markup::escape_text(m_value) + "\"";
}

const Glib::ustring &Attribute::getName() const
{
  return m_name;
}

const Glib::ustring &Attribute::getValue() const
{
  return m_value;
}

Glib::ustring attribute_detail::toString(const ParameterId &v)
{
  return v.toString();
}

Glib::ustring attribute_detail::toString(const GroupId &v)
{
  return v.toString();
}

Glib::ustring attribute_detail::toString(const Glib::ustring &v)
{
  return v;
}
