#include <Application.h>
#include <playground.h>
#include <boost/tokenizer.hpp>
#include <parameters/names/ParameterDB.h>
#include <tools/StringTools.h>
#include "RowStream.h"
#include <parameters/Parameter.h>

ParameterDB &ParameterDB::get()
{
  static ParameterDB db;
  return db;
}

ParameterDB::ParameterDB()
{
  read();
}

ParameterDB::~ParameterDB()
{
}

void ParameterDB::read()
{
  RowStream in(Application::get().getResourcePath() + "ParameterList.csv");
  in.eatRow();
  in.forEach(std::bind(&ParameterDB::parseCSVRow, this, std::placeholders::_1));
}

void ParameterDB::parseCSVRow(const std::string &row)
{
  try
  {
    importParsedRow(textRowToVector(row));
  }
  catch(...)
  {
  }
}

std::vector<std::string> ParameterDB::textRowToVector(const std::string &row) const
{
  typedef boost::escaped_list_separator<char> tSeparator;
  typedef boost::tokenizer<tSeparator> tTokenizer;
  tTokenizer tok(row, tSeparator('\\', ',', '\"'));
  return std::vector<std::string>(tok.begin(), tok.end());
}

std::string sanitize(const std::string &in)
{
  auto mod = StringTools::replaceAll(in, "Ⓐ", u8"\ue000");
  mod = StringTools::replaceAll(mod, "Ⓑ", u8"\ue001");
  mod = StringTools::replaceAll(mod, "Ⓒ", u8"\ue002");
  mod = StringTools::replaceAll(mod, "Ⓓ", u8"\ue003");
  mod = StringTools::replaceAll(mod, "Ⓔ", u8"\ue0004");
  mod = StringTools::replaceAll(mod, "Ⓕ", u8"\ue0005");
  return mod;
}

void ParameterDB::importParsedRow(std::vector<std::string> &&items)
{
  if(!items[0].empty())
  {
    int id = std::stoi(items[0]);
    m_spec[id] = { sanitize(items[4]), sanitize(items[8]), parseSignalPathIndication(items[11]) };
  }
}

tControlPositionValue ParameterDB::parseSignalPathIndication(const std::string &c) const
{
  if(!c.empty())
  {
    try
    {
      return std::stod(c);
    }
    catch(...)
    {
    }
  }
  return getInvalidSignalPathIndication();
}

ustring ParameterDB::getLongName(int id) const
{
  try
  {
    return m_spec.at(id).longName;
  }
  catch(...)
  {
    return std::to_string(id) + " not found";
  }
}

ustring ParameterDB::getShortName(int id) const
{
  try
  {
    return m_spec.at(id).shortName;
  }
  catch(...)
  {
    return std::to_string(id) + " not found";
  }
}

tControlPositionValue ParameterDB::getSignalPathIndication(int id) const
{
  try
  {
    return m_spec.at(id).signalPathIndication;
  }
  catch(...)
  {
    return 0;
  }
}

bool ParameterDB::isActive(const Parameter *p) const
{
  const auto inActiveCP = getSignalPathIndication(p->getID());
  const auto diff = std::abs(inActiveCP - p->getControlPositionValue());
  return diff > std::numeric_limits<tControlPositionValue>::epsilon();
}
