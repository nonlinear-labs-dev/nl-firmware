#include <Application.h>
#include "BankSorter.h"
#include "PresetManager.h"
#include "PresetBank.h"

void BankSorter::sort()
{
  auto scope = getPM()->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();
  auto newBankOrder = concatVectorsAccordingToRule(getClusterVectorsFromClusterMasters(getClusterMastersSortedByX()), getFreeBanksSortedByX());

  int index = getPM()->getNumBanks() - 1;
  for(auto it = newBankOrder.rbegin(); it < newBankOrder.rend(); it++) {
    getPM()->undoableSetOrderNumber(transaction, *it, index--);
  }
}

vector<BankSorter::tPresetBankPtr>
BankSorter::concatVectorsAccordingToRule(const vector<BankSorter::tPresetBankPtr> &clusters,
                                         const vector<BankSorter::tPresetBankPtr> &freeBanks) {
  vector<tPresetBankPtr> newBankOrder;
  for(auto clusterBanks: clusters) {
    newBankOrder.push_back(clusterBanks);
  }
  for(auto bank: freeBanks) {
    newBankOrder.push_back(bank);
  }
  return newBankOrder;
}

std::shared_ptr<PresetManager> BankSorter::getPM() {
  return Application::get().getPresetManager();
}

vector<BankSorter::tPresetBankPtr> BankSorter::getClusterMastersSortedByX() {
  std::vector<tPresetBankPtr> clusterMasters;
  for(auto bank: getPM()->getBanks()) {
    if(bank->isInCluster()) {
      if(auto master = bank->getClusterMaster()) {
        if(master != bank) {
          if(std::find(clusterMasters.begin(), clusterMasters.end(), master) == clusterMasters.end())  {
            clusterMasters.push_back(master);
          }
        }
      }
    }
  }

  std::sort(clusterMasters.begin(), clusterMasters.end(), [](const std::shared_ptr<PresetBank> lhs,
                                                           const std::shared_ptr<PresetBank> rhs) {
    return stoi(lhs->getX()) < stoi(rhs->getX());
  });

  return clusterMasters;
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

  vector<PresetBank*> debugBank;
  for(auto bank: banks) {
    debugBank.push_back(bank.get());
  }


  std::sort(banks.begin(), banks.end(), [](const std::shared_ptr<PresetBank> lhs,
                                           const std::shared_ptr<PresetBank> rhs) {
    return stoi(lhs->getX()) < stoi(rhs->getX());
  });

  return banks;
}

vector<BankSorter::tPresetBankPtr> BankSorter::getClusterVectorsFromClusterMasters(vector<tPresetBankPtr> clusterMasters) {
  std::vector<BankSorter::tPresetBankPtr> ret;
  for(auto clusterMaster: clusterMasters) {
    for(auto bank: clusterMaster->getClusterAsSortedVector()) {
      ret.push_back(bank);
    }
  }
  return ret;
}

