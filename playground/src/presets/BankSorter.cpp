#include <Application.h>
#include "BankSorter.h"
#include "PresetManager.h"
#include "PresetBank.h"

void BankSorter::sort()
{
  auto scope = getPM()->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();
  auto clusters = getClusterMastersSortedByX();
  auto freeBanks = getFreeBanksSortedByX();

  vector<tPresetBankPtr> newBankOrder;
  for(auto clusterMaster: clusters) {
    for(auto bank: clusterMaster->getClusterAsSortedVector()) {
      newBankOrder.push_back(bank);
    }
  }
  for(auto bank: freeBanks) {
    newBankOrder.push_back(bank);
  }
  getPM()->undoableSetBanks(transaction, newBankOrder);
}

std::shared_ptr<PresetManager> BankSorter::getPM() {
  return Application::get().getPresetManager();
}

vector<BankSorter::tPresetBankPtr> BankSorter::getClusterMastersSortedByX() {
  std::vector<tPresetBankPtr> clusterMaster;
  for(auto bank: getPM()->getBanks()) {
    if(bank->isInCluster()) {
      if(auto master = bank->getClusterMaster()) {
        if(master != bank) {
          if(std::find(clusterMaster.begin(), clusterMaster.end(), master) == clusterMaster.end())  {
            clusterMaster.push_back(master);
          }
        }
      }
    }
  }

  std::sort(clusterMaster.begin(), clusterMaster.end(), [](const std::shared_ptr<PresetBank> lhs,
                                                           const std::shared_ptr<PresetBank> rhs) {
    return lhs->getX() > rhs->getX();
  });

  return clusterMaster;
}

vector<BankSorter::tPresetBankPtr> BankSorter::getFreeBanksSortedByX() {
  std::vector<tPresetBankPtr> banks;
  for(auto bank: getPM()->getBanks()) {
    if(!bank->isInCluster()) {
        if(std::find(banks.begin(), banks.end(), bank) == banks.end())  {
          banks.push_back(bank);
        }
    }
  }

  std::sort(banks.begin(), banks.end(), [](const std::shared_ptr<PresetBank> lhs,
                                                           const std::shared_ptr<PresetBank> rhs) {
    return lhs->getX() > rhs->getX();
  });

  return banks;
}

