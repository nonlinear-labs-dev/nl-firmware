#include <Application.h>
#include "StoreModeData.h"
#include "PresetManager.h"
#include "PresetBank.h"

StoreModeData::StoreModeData() {
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank()) {
    bankPos = pm->calcBankIndex(bank.get());
    auto selected = bank->getSelectedPreset();
    presetPos = static_cast<int>(bank->getPresetPosition(selected));
  }
}
