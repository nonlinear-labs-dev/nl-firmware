#include <Application.h>
#include "StoreModeData.h"
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <algorithm>

StoreModeData::StoreModeData()
{
  if(auto pm = Application::get().getPresetManager())
  {
    const auto &banks = pm->getBanks();
    auto it = std::find(banks.begin(), banks.end(), pm->getSelectedBank());
    if(it != banks.end())
    {
      auto &bank = *it;
      bankPos = static_cast<size_t>(std::distance(banks.begin(), it));

      if(bank->getSelectedPresetUuid().empty())
        presetPos = invalid;
      else
        presetPos = (*it)->getPresetPosition(bank->getSelectedPresetUuid());
    }
  }
}
