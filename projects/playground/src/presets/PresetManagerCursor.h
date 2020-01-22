#pragma once

#include "tools/Uuid.h"
#include <functional>

class Preset;
class Bank;

class PresetManagerCursor
{
 public:
  using Matcher = std::function<bool(const Preset*)>;

  PresetManagerCursor(Matcher matcher = [](auto) { return true; });

  void moveToSelected();

  bool nextBank();
  bool canNextBank() const;

  bool previousBank();
  bool canPreviousBank() const;

  bool nextPreset();
  bool canNextPreset() const;

  bool previousPreset();
  bool canPreviousPreset() const;

  Preset* getPreset() const;
  Uuid getPresetUuid() const;
  Bank* getBank() const;
  Uuid getBankUuid() const;

  std::string getPresetNumberString() const;
  std::string getPresetName() const;
  std::string getPresetType() const;
  std::string getBankName() const;

 private:
  bool advanceBank(int i);
  bool advancePreset(int i);
  bool canAdvanceBank(int i) const;

  Preset* findFirstMatchingPreset(Bank* bank) const;
  Preset* findLastMatchingPreset(Bank* bank) const;

  Matcher m_matcher;
  Uuid m_preset;
  Uuid m_bank;
};
