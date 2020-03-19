#include "LoadToPartPresetList.h"

LoadToPartPresetList::LoadToPartPresetList(const Rect& pos, bool showBankArrows)
    : PresetList(pos, showBankArrows)
{
}
void LoadToPartPresetList::onRotary(int inc, ButtonModifiers modifiers)
{
  PresetList::onRotary(inc, modifiers);
}

LoadToPartPresetList::~LoadToPartPresetList() = default;
