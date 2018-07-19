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
    static vector<tPresetBankPtr> concatVectorsAccordingToRule(const vector<tPresetBankPtr> &clusters,
                                                               const vector<tPresetBankPtr> &freeBanks);
    static vector<tPresetBankPtr> getClusterVectorsFromClusterMasters(vector<tPresetBankPtr> clusterMasters);
};
