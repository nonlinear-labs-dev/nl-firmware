#pragma once
#include "PresetBank.h"

class BankChangeBlocker {
public:
  BankChangeBlocker(std::shared_ptr<PresetBank> bank);
  ~BankChangeBlocker();
protected:
  std::shared_ptr<PresetBank> m_bank;
};