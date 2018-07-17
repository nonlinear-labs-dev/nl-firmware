#include <Application.h>
#include "BankSorter.h"
#include "PresetManager.h"
#include "PresetBank.h"

void BankSorter::sort()
{
  auto scope = getPM()->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();
  int orderNumber = 0;
  auto clusters = getClusterMastersSortedByX();
  orderNumber = reasignOrderNumberForCluster(transaction, clusters, orderNumber);

}

std::shared_ptr<PresetManager> BankSorter::getPM() {
  return Application::get().getPresetManager();
}

vector<BankSorter::tPresetBankPtr> BankSorter::getClusterMastersSortedByX() {
  std::vector<tPresetBankPtr> clusterMaster;
  for(auto bank: getPM()->getBanks()) {
    if(bank->isInCluster()) {
      if(auto master = bank->getClusterMaster()) {
        if(master.get() != bank.get()) {
          if(std::find(clusterMaster.begin(), clusterMaster.end(), master) == clusterMaster.end())  {
            clusterMaster.push_back(master);
          }
        }
      }
    }
  }

  std::sort(clusterMaster.begin(), clusterMaster.end(), [](const std::shared_ptr<PresetBank> lhs,
                                                           const std::shared_ptr<PresetBank> rhs) {
    return lhs->getX() < rhs->getX();
  });

  return clusterMaster;
}

int BankSorter::reasignOrderNumberForCluster(UNDO::Scope::tTransactionPtr transaction,
                                             vector<tPresetBankPtr> vector,
                                             int startFrom) {
  int ret = startFrom;
  for(auto clusterMaster: vector) {
    ret = reasignOrderNumberInCluster(transaction, clusterMaster, startFrom);
  }
  return ret;
}

int BankSorter::reasignOrderNumberInCluster(UNDO::Scope::tTransactionPtr transaction,
                                            tPresetBankPtr master,
                                            int startWith)
{
  for(auto bank: master->getClusterAsSortedVector()) {
    getPM()->undoableSetOrderNumber(transaction, bank, startWith++);
  }

  return startWith;
}

