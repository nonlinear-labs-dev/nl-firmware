#pragma once

#include "playground.h"
#include "Ribbon.h"
#include <nltools/messaging/Message.h>
#include <sigc++/trackable.h>

class Application;
class Parameter;
class Setting;

class LowerRibbon : public Ribbon, public sigc::trackable
{
  typedef Ribbon super;

 public:
  LowerRibbon();
  void indicateBlockingMainThread(bool onOff);

 private:
  void onParamSelectionChanged(const Parameter* old, const Parameter* newP);
  void onParamValueChanged(const Parameter* param);
  void onSendValueChanged(const Parameter *param);
  void reconnect();

  int posToLedID(int pos) const override;

  bool m_indicateBlockingMainThread = false;

  sigc::connection m_paramConnection;
  sigc::connection m_sendConnection;

  sigc::connection m_parameterSelectionChangedSignal;
  sigc::connection m_settingChangedSignal;
};
