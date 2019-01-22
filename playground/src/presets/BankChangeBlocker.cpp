#include "BankChangeBlocker.h"
#include <presets/Bank.h>

BankChangeBlocker::BankChangeBlocker(Bank *bank)
    : m_bank(bank)
{
  m_bank->addChangeBlocker();
}

BankChangeBlocker::~BankChangeBlocker()
{
  m_bank->removeChangeBlocker();
}
