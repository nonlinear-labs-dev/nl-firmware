#include <presets/EditBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <nltools/Types.h>
#include <libundo/undo/Scope.h>
#include "ConvertToSoundTypeItem.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include "proxies/hwui/descriptive-layouts/ConditionRegistry.h"

namespace
{
  std::string getSuffix(SoundType convertTo)
  {
    auto currentType = Application::get().getPresetManager()->getEditBuffer()->getType();
    if((currentType != SoundType::Single && convertTo == SoundType::Single)
       || (currentType == SoundType::Single && convertTo != SoundType::Single))
      return " (both FX)";
    else
      return "";
  }
}

ConvertToSoundTypeItem::ConvertToSoundTypeItem(const Rect& rect, SoundType toType)
    : AnimatedGenericItem(nltools::string::concat("Convert to ", toString(toType), getSuffix(toType)), rect,
                          OneShotTypes::StartCB([=] {
                            m_lock = ConditionRegistry::createLock();
                            EditBufferUseCases useCases(*Application::get().getPresetManager()->getEditBuffer());
                            auto selectedVG = Application::get().getVGManager()->getCurrentVoiceGroup();

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
                          }),
                          OneShotTypes::FinishCB([=]() {
                            m_lock.reset();
                            SettingsUseCases sUseCases(*Application::get().getSettings());
                            sUseCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                          }))
{
}

ConvertToSoundTypeItemWithFX::ConvertToSoundTypeItemWithFX(const Rect& rect, SoundType convertTo)

    : AnimatedGenericItem(nltools::string::concat("Convert to ", toString(convertTo), " (FX I only)"), rect,
                          OneShotTypes::StartCB([=] {
                            m_lock = ConditionRegistry::createLock();
                            EditBufferUseCases useCases(*Application::get().getPresetManager()->getEditBuffer());

                            switch(convertTo)
                            {
                              case SoundType::Layer:
                              {
                                useCases.convertSingleToLayerFXIOnly();
                                break;
                              }
                              case SoundType::Split:
                              {
                                useCases.convertSingleToSplitFXIOnly();
                                break;
                              }
                              default:
                                break;
                            }
                          }),
                          OneShotTypes::FinishCB([=] {
                            m_lock.reset();
                            SettingsUseCases sUseCases(*Application::get().getSettings());
                            sUseCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
                          }))
{
}
