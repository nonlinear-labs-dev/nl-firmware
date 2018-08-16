#pragma once

#include "PresetManager.h"

class PresetBank;

class BankSorter {
public:
    typedef PresetManager::tBankPtr tPresetBankPtr;
    typedef vector<tPresetBankPtr> BankVector;
    static void sort();
};
