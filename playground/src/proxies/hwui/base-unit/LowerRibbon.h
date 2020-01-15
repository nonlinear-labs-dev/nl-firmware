#pragma once

#include "playground.h"
#include "Ribbon.h"
#include <sigc++/connection.h>

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

  static Parameter* getResponsibleParameter();

  int posToLedID(int pos) const;

  sigc::connection m_paramConnection;
  bool m_indicateBlockingMainThread = false;
};
