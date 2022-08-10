#include "RandomizeItem.h"
#include "use-cases/EditBufferUseCases.h"
#include <Application.h>
#include <device-settings/RandomizeAmount.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/SettingLabel.h>

#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

#include <libundo/undo/Scope.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ChangeSettingWithCommitOverlay.h>

RandomizeItem::RandomizeItem(const Rect& rect)
    : EditorItem("Randomize Sound", rect)
{
  auto rightHalf = getPosition();
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(rightHalf.getWidth());

  addControl(new SettingLabel<RandomizeAmount>(rightHalf));
}

RandomizeItem::~RandomizeItem() = default;

MenuOverlay* RandomizeItem::createOverlay()
{
  return new ChangeSettingWithCommitOverlay<RandomizeAmount>(
      getDefaultOverlayRect(),
      []
      {
        auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();
        auto pm = Application::get().getPresetManager();
        auto eb = pm->getEditBuffer();
        EditBufferUseCases useCases(*eb);
        useCases.randomize(amount);
      });
}

RandomizePart::RandomizePart(const Rect& r)
    : EditorItem("Randomize Part", r)
{
  auto rightHalf = getPosition();
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(rightHalf.getWidth());

  addControl(new SettingLabel<RandomizeAmount>(rightHalf));
}

RandomizePart::~RandomizePart() = default;

MenuOverlay* RandomizePart::createOverlay()
{
  return new ChangeSettingWithCommitOverlay<RandomizeAmount>(
      getDefaultOverlayRect(),
      []
      {
        auto amount = Application::get().getSettings()->getSetting<RandomizeAmount>()->get();
        auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
        auto pm = Application::get().getPresetManager();
        auto eb = pm->getEditBuffer();
        EditBufferUseCases useCases(*eb);
        useCases.randomizePart(vg, amount);
      });
}
