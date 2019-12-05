#include <Application.h>
#include <playground.h>
#include <boost/tokenizer.hpp>
#include <parameters/names/ParameterDB.h>
#include <tools/StringTools.h>
#include "RowStream.h"
#include <parameters/Parameter.h>
#include <parameter-db/generated/parameter_list.h>
#include <assert.h>
#include <nltools/logging/Log.h>

ParameterDB &ParameterDB::get()
{
  static ParameterDB db;
  return db;
}

ParameterDB::ParameterDB() = default;
ParameterDB::~ParameterDB() = default;

std::string sanitize(const std::string &in)
{
  auto mod = StringTools::replaceAll(in, "Ⓐ", u8"\ue000");
  mod = StringTools::replaceAll(mod, "Ⓑ", u8"\ue001");
  mod = StringTools::replaceAll(mod, "Ⓒ", u8"\ue002");
  mod = StringTools::replaceAll(mod, "Ⓓ", u8"\ue003");
  mod = StringTools::replaceAll(mod, "Ⓔ", u8"\ue200");
  mod = StringTools::replaceAll(mod, "Ⓕ", u8"\ue201");
  return mod;
}

Glib::ustring ParameterDB::getLongName(int id) const
{
  assert(id >= 0);
  assert(id < C15::Config::tcd_elements);

  auto d = C15::ParameterList[id];
  if(!d.m_pg.m_param_label_long)
  {
    nltools::Log::error("there is no long name entry in parameter list for parameter", id);
    return "MISSING!!!";
  }
  return d.m_pg.m_param_label_long;
}

Glib::ustring ParameterDB::getShortName(int id) const
{
  assert(id >= 0);
  assert(id < C15::Config::tcd_elements);

  auto d = C15::ParameterList[id];
  if(!d.m_pg.m_param_label_short)
  {
    nltools::Log::error("there is no short name entry in parameter list for parameter", id);
    return "MISSING!!!";
  }
  return d.m_pg.m_param_label_short;
}

tControlPositionValue ParameterDB::getSignalPathIndication(int id) const
{
  assert(id >= 0);
  assert(id < C15::Config::tcd_elements);

  auto d = C15::ParameterList[id];
  return (d.m_pg.m_inactive_cp && strlen(d.m_pg.m_inactive_cp) > 0) ? std::stod(d.m_pg.m_inactive_cp) : 0;
}

bool ParameterDB::isActive(const Parameter *p) const
{
  const auto inActiveCP = getSignalPathIndication(p->getID().getNumber());
  const auto diff = std::abs(inActiveCP - p->getControlPositionValue());
  return diff > std::numeric_limits<tControlPositionValue>::epsilon();
}
