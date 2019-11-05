#pragma once

#include <presets/Bank.h>
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/panel-unit/boled/info/InfoContent.h>

class MultiLineLabel;
class Label;
class Uuid;

class BankInfoContent : public InfoContent
{
 private:
  typedef InfoContent super;

 public:
  BankInfoContent();

  void fillContents() override;

 private:
  void onBankSelectionChanged(const Uuid &selectedBank);
  void onBankChanged(Bank *bank);

  void fillFromBank(Bank *bank);

  sigc::connection m_bankConnection;
};
