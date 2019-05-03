#pragma once

#include "playground.h"

class Parameter;

class ParameterDB
{
 public:
  static ParameterDB &get();

  virtual ~ParameterDB();

  ustring getLongName(int id) const;
  ustring getShortName(int id) const;
  tControlPositionValue getSignalPathIndication(int id) const;

  static constexpr tControlPositionValue getInvalidSignalPathIndication()
  {
    return std::numeric_limits<tControlPositionValue>::max();
  }

  bool isActive(const Parameter *p) const;

 private:
  ParameterDB();

  void read();
  void parseCSVRow(const std::string &row);
  std::vector<std::string> textRowToVector(const std::string &row) const;
  void importParsedRow(std::vector<std::string> &&items);
  tControlPositionValue parseSignalPathIndication(const std::string &c) const;

  struct ParameterSpec
  {
    ustring longName;
    ustring shortName;
    tControlPositionValue signalPathIndication;
  };

  std::map<int, ParameterSpec> m_spec;
};
