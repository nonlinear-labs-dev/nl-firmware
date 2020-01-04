#include "Attribute.h"
#include <glibmm.h>

Attribute::~Attribute() = default;

std::string Attribute::toXmlString() const
{
  return Glib::Markup::escape_text(m_name) + "=\"" + Glib::Markup::escape_text(m_value) + "\"";
}

const std::string &Attribute::getName() const
{
  return m_name;
}

const std::string &Attribute::getValue() const
{
  return m_value;
}
