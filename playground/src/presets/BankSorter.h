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
    static vector<tPresetBankPtr> getFreeBanksSortedByX();
};
