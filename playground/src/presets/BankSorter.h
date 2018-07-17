#pragma once

#include <libundo/undo/Scope.h>
#include "PresetManager.h"

class PresetBank;

class BankSorter {
public:
    typedef PresetManager::tBankPtr tPresetBankPtr;
    static void sort();
private:
    static shared_ptr<PresetManager> getPM();
    static vector<tPresetBankPtr> getClusterMastersSortedByX();
    static int reasignOrderNumberForCluster(UNDO::Scope::tTransactionPtr transaction, vector<tPresetBankPtr> vector, int number);
    static int reasignOrderNumberInCluster(UNDO::Scope::tTransactionPtr transaction, tPresetBankPtr master, int startWith);
};
