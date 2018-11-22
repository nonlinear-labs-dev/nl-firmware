#pragma once

#include "playground.h"
#include <map>
#include <list>

class ButtonParameterMapping
{
 public:
  ButtonParameterMapping();
  virtual ~ButtonParameterMapping();

  void forEachButton(function<void(int, const list<int> &)> cb);
  int findButton(int param) const;
  list<int> findParameters(int button) const;

 private:
  void addMapping(int buttonID, initializer_list<int> parameterIDs);

  map<int, int> m_paramIDToButtonID;
  map<int, list<int>> m_buttonIDToParam;
};
