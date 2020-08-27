#pragma once

#include <nltools/Types.h>
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <memory>
#include <set>

class Preset;

namespace PresetAlgorithms
{
  /* iterates all parameters of 'left' and asks 'cb' to decide whether to stick with the value in 'left' or replace it
 * with the value of 'right'. */

  std::unique_ptr<Preset> merge(UNDO::Transaction *transaction, std::unique_ptr<Preset> left,
                                std::unique_ptr<Preset> right, const std::set<int> &takeFromRight)
  {
    for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
      for(auto &leftGroup : left->getGroups(vg))
        for(auto &leftParameter : leftGroup.second->getParameters())
          if(takeFromRight.count(leftParameter.first.getNumber()))
            if(auto rightParameter = right->findParameterByID(leftParameter.first, false))
              leftParameter.second->copyFrom(transaction, rightParameter);

    return left;
  }
}
