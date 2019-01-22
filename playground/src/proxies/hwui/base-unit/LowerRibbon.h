#pragma once

#include "playground.h"
#include "Ribbon.h"
#include "parameters/Parameter.h"

class Application;
class Parameter;
class Setting;

class LowerRibbon : public Ribbon
{
  typedef Ribbon super;

 public:
  LowerRibbon();
  void indicateBlockingMainThread(bool onOff);

 private:
  void onParamSelectionChanged(Parameter* oldOne, Parameter* newOne);
  void onParamValueChanged(const Parameter* param);
  void reconnect();

  Parameter* getResponsibleParameter();

  int posToLedID(int pos) const;

  sigc::connection m_paramConnection;
  bool m_indicateBlockingMainThread = false;
};
