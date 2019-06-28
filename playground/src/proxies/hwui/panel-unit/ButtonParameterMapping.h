#pragma once

#include "playground.h"
#include <map>
#include <list>

class ButtonParameterMapping
{
 public:
  ButtonParameterMapping();
  virtual ~ButtonParameterMapping();

  void forEachButton(std::function<void(int, const std::list<int> &)> cb);
  int findButton(int param) const;
  std::list<int> findParameters(int button) const;

 private:
  void addMapping(int buttonID, std::initializer_list<int> parameterIDs);

  std::map<int, int> m_paramIDToButtonID;
  std::map<int, std::list<int>> m_buttonIDToParam;
};
