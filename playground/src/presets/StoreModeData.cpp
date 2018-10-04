#include <Application.h>
#include "StoreModeData.h"
#include "PresetManager.h"
#include "PresetBank.h"

StoreModeData::StoreModeData() {
  if(auto pm = Application::get().getPresetManager()) {

    if(auto banks = pm->getBanks(); !banks.empty())
      bankPos = static_cast<int>(std::find(banks.begin(), banks.end(), pm->getSelectedBank()) - banks.begin());

    if(auto bank = pm->getBank(bankPos))
      presetPos = static_cast<int>(bank->getPresetPosition(bank->getSelectedPreset()));
  }

}
