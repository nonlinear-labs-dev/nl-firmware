#pragma once

#include "proxies/hwui/controls/ButtonMenu.h"
#include <ParameterId.h>

class PresetBank;
class Parameter;
class ParameterGroup;

class ParameterEditButtonMenu : public ButtonMenu
{
 private:
  typedef ButtonMenu super;

 public:
  explicit ParameterEditButtonMenu(const Rect &rect);
  ~ParameterEditButtonMenu() override;
  void selectButton(size_t i) override;

 protected:
  void setup();
  virtual void addActions();
  virtual Parameter *getSelectedParameter();

 private:
  void toggleGroupLock();
  void unlockAll();
  void lockAll();
  void onParameterSelectionChanged(Parameter *oldParameter, Parameter *newParameter);
  void onGroupChanged();
  void sanitizeLastAction();
  void selectParameter(ParameterId id);

  ParameterGroup *m_currentGroup = nullptr;
  bool m_allParametersLocked = false;
  sigc::connection m_connection;

  static int s_lastAction;
};