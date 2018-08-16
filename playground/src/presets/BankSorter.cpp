#include <Application.h>
#include <tools/PerformanceTimer.h>
#include "BankSorter.h"
#include "PresetManager.h"
#include "ClusterEnforcement.h"

void BankSorter::sort()
{
  PerformanceTimer t("Sort Banks");

  auto scope = Application::get().getPresetManager()->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();

  ClusterEnforcement ce;
  auto newBanks = ce.sortBanks();
  Application::get().getPresetManager()->undoableSetBanks(transaction, newBanks);
}