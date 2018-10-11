#include "BankChangeBlocker.h"

BankChangeBlocker::BankChangeBlocker(std::shared_ptr<PresetBank> bank) : m_bank(bank) {
  m_bank->addChangeBlocker(this);
}

BankChangeBlocker::~BankChangeBlocker() {
  m_bank->removeChangeBlocker(this);
}
