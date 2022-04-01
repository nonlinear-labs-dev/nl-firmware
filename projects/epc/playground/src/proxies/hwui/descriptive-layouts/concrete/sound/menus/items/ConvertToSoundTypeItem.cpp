#include <presets/EditBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <nltools/Types.h>
#include <libundo/undo/Scope.h>
#include "ConvertToSoundTypeItem.h"

ConvertToSoundTypeItem::ConvertToSoundTypeItem(const Rect& rect, SoundType toType)
    : AnimatedGenericItem(nltools::string::concat("Convert to ", toString(toType)), rect, [=] {
      EditBufferUseCases useCases(*Application::get().getPresetManager()->getEditBuffer());
      auto selectedVG = Application::get().getHWUI()->getCurrentVoiceGroup();

      switch(toType)
      {
        case SoundType::Single:
        {
          useCases.convertToSingle(selectedVG);
          break;
        }
        case SoundType::Split:
        {
          useCases.convertToSplit(selectedVG);
          break;
        }
        case SoundType::Layer:
        {
          useCases.convertToLayer(selectedVG);
          break;
        }
        default:
          break;
      }

      Application::get().getHWUI()->undoableSetFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
    })
{
}
