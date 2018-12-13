#pragma once

class Bank;

class BankChangeBlocker
{
 public:
  BankChangeBlocker(Bank* bank);
  ~BankChangeBlocker();

 protected:
  Bank* m_bank;
};
