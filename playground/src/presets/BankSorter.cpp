#include <Application.h>
#include <tools/PerformanceTimer.h>
#include "BankSorter.h"
#include "PresetManager.h"
#include "PresetBank.h"
#include "ClusterEnforcement.h"

bool inSameCluster(const std::shared_ptr<PresetBank>& lhs, const std::shared_ptr<PresetBank>& rhs) {
  return lhs->getClusterMaster() == rhs->getClusterMaster();
}

void BankSorter::sort()
{
  PerformanceTimer t("Sort Banks");

  auto scope = getPM()->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();

  ClusterEnforcement ce;
  auto newBanks = ce.sortBanks(getPM()->getBanks());
  Application::get().getPresetManager()->undoableSetBanks(transaction, newBanks);
}

BankSorter::BankVector BankSorter::concatVectorsAccordingToRule(BankVector clusters,
                                         const BankVector &freeBanks) {
  clusters.insert( clusters.end(), freeBanks.begin(), freeBanks.end() );
  return clusters;
}

std::shared_ptr<PresetManager> BankSorter::getPM() {
  return Application::get().getPresetManager();
}

BankSorter::BankVector BankSorter::getClusterMastersSortedByX() {
  BankVector clusterMasters;

  for(auto bank: getPM()->getBanksIf([](auto bank){return bank->isInCluster();}))
  {
    if(auto master = bank->getClusterMaster()) {
      if(isValidMasterBank(bank.get(), master)) {
        if(doesNotContain(clusterMasters, master->shared_from_this()))  {
          clusterMasters.push_back(master->shared_from_this());
        }
      }
    }
  }
  std::sort(clusterMasters.begin(), clusterMasters.end(), compareByXPosition);

  return clusterMasters;
}

BankSorter::BankVector BankSorter::getFreeBanksSortedByX() {
  BankVector banks;

  for(auto bank: getPM()->getBanks()) {
    if(!bank->isInCluster()) {
      if(doesNotContain(banks, bank)) {
        banks.push_back(bank);
      }
    }
  }
  std::sort(banks.begin(), banks.end(), compareByXPosition);

  return banks;
}

BankSorter::BankVector BankSorter::getClusterVectorsFromClusterMasters(BankSorter::BankVector clusterMasters) {
  BankVector ret;
  for(auto clusterMaster: clusterMasters) {
    for(auto bank: clusterMaster->getClusterAsSortedVector()) {
      ret.push_back(bank);
    }
  }
  return ret;
}

bool BankSorter::doesNotContain(BankVector &clusterMasters,
                                BankSorter::tPresetBankPtr master) {
  return find(clusterMasters.begin(), clusterMasters.end(), master) == clusterMasters.end();
}

bool BankSorter::isValidMasterBank(PresetBank* bank, PresetBank* master) {
  return master != bank;
}

bool BankSorter::compareByXPosition(const tPresetBankPtr &lhs, const tPresetBankPtr &rhs) {
  return stoi(lhs->getX()) < stoi(rhs->getX());
}


