#include "EditBufferEvents.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <groups/ParameterGroup.h>
#include <sigc++/sigc++.h>
#include <parameters/SplitPointParameter.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>

void DescriptiveLayouts::EditBufferTypeStringEvent::onChange(const EditBuffer *eb)
{
  const auto type = eb->getType();
  const auto typeStr = toString(type);
  setValue({ typeStr + " Sound", 0 });
}

void DescriptiveLayouts::EditBufferName::onChange(const EditBuffer *eb)
{
  setValue({ eb->getName(), 0 });
}

void DescriptiveLayouts::EditBufferNameWithSuffix::onChange(const EditBuffer *eb)
{
  setValue({ eb->getNameWithSuffix(), 0 });
}

void DescriptiveLayouts::CurrentVoiceGroupLabel::onChange(const EditBuffer *eb)
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  setValue({ eb->getVoiceGroupNameWithSuffix(currentVG, false), 0 });
}

void DescriptiveLayouts::IsCurrentVGI::onChange(const EditBuffer *eb)
{
  auto val = Application::get().getVGManager()->getCurrentVoiceGroup();
  setValue(val == VoiceGroup::I);
}

void DescriptiveLayouts::IsCurrentVGII::onChange(const EditBuffer *eb)
{
  auto val = Application::get().getVGManager()->getCurrentVoiceGroup();
  setValue(val == VoiceGroup::II);
}

void DescriptiveLayouts::SoundParamsButtonText::onChange(const EditBuffer *eb)
{
  auto base = "Part..";
  auto suffix = isChanged(eb) ? "*" : "";
  setValue({ std::string(base) + suffix, 0 });
}

bool DescriptiveLayouts::SoundParamsButtonText::isChanged(const EditBuffer *eb)
{
  auto type = eb->getType();
  switch(type)
  {
    case SoundType::Invalid:
    case SoundType::Single:
    default:
      return false;
    case SoundType::Layer:
    case SoundType::Split:
    {
      auto vgI = eb->getParameterGroupByID({ "Part", VoiceGroup::I })->isAnyParameterChanged();
      auto vgII = eb->getParameterGroupByID({ "Part", VoiceGroup::II })->isAnyParameterChanged();
      return vgI || vgII;
    }
  }
}

void DescriptiveLayouts::SoundVoicesButtonText::onChange(const EditBuffer *eb)
{
  auto base = "Voices..";
  auto suffix = isChanged(eb) ? "*" : "";
  setValue({ std::string(base) + suffix, 0 });
}

bool DescriptiveLayouts::SoundVoicesButtonText::isChanged(const EditBuffer *eb)
{
  auto type = eb->getType();
  if(type == SoundType::Split)
  {
    auto monoI = eb->getParameterGroupByID({ "Mono", VoiceGroup::I });
    auto monoII = eb->getParameterGroupByID({ "Mono", VoiceGroup::II });
    auto vgUnisonI = eb->getParameterGroupByID({ "Unison", VoiceGroup::I });
    auto vgUnisonII = eb->getParameterGroupByID({ "Unison", VoiceGroup::II });
    return (monoI && monoI->isAnyParameterChanged()) || (monoII && monoII->isAnyParameterChanged())
        || (vgUnisonI && vgUnisonI->isAnyParameterChanged()) || (vgUnisonII && vgUnisonII->isAnyParameterChanged());
  }
  else if(type == SoundType::Layer || type == SoundType::Single)
  {
    auto mono = eb->getParameterGroupByID({ "Mono", VoiceGroup::I });
    auto vgUnison = eb->getParameterGroupByID({ "Unison", VoiceGroup::I });
    return (mono && mono->isAnyParameterChanged()) || (vgUnison && vgUnison->isAnyParameterChanged());
  }
  return false;
}

void DescriptiveLayouts::SoundMasterButtonText::onChange(const EditBuffer *eb)
{
  auto base = "Master..";
  auto suffix = isChanged(eb) ? "*" : "";
  setValue({ std::string(base) + suffix, 0 });
}

bool DescriptiveLayouts::SoundMasterButtonText::isChanged(const EditBuffer *eb)
{
  auto masterGroup = eb->getParameterGroupByID({ "Master", VoiceGroup::Global });
  auto scale = eb->getParameterGroupByID({ "Scale", VoiceGroup::Global });

  bool masterChanged = false;
  for(auto masterParams : masterGroup->getParameters())
    if(masterParams->getID().getNumber() != C15::PID::Master_FX_Mix)
      masterChanged |= masterParams->isChangedFromLoaded();

  return (scale && scale->isAnyParameterChanged()) || masterChanged;
}

void DescriptiveLayouts::MonoButtonText::onChange(const EditBuffer *eb)
{
  auto base = "Mono..";
  auto suffix = isChanged(eb) ? "*" : "";
  setValue({ std::string(base) + suffix, 0 });
}

bool DescriptiveLayouts::MonoButtonText::isChanged(const EditBuffer *eb)
{
  switch(eb->getType())
  {
    case SoundType::Single:
    case SoundType::Invalid:
    case SoundType::Layer:
    {
      auto mono = eb->getParameterGroupByID({ "Mono", VoiceGroup::I });
      return mono && mono->isAnyParameterChanged();
    }
    case SoundType::Split:
    {
      auto mono = eb->getParameterGroupByID({ "Mono", Application::get().getVGManager()->getCurrentVoiceGroup() });
      return mono && mono->isAnyParameterChanged();
    }
  }
  return false;
}

void DescriptiveLayouts::UnisonButtonText::onChange(const EditBuffer *eb)
{
  auto base = "Unison..";
  auto suffix = isChanged(eb) ? "*" : "";
  setValue({ std::string(base) + suffix, 0 });
}

bool DescriptiveLayouts::UnisonButtonText::isChanged(const EditBuffer *eb)
{
  switch(eb->getType())
  {
    case SoundType::Single:
    case SoundType::Invalid:
    case SoundType::Layer:
    {
      auto unison = eb->getParameterGroupByID({ "Unison", Application::get().getVGManager()->getCurrentVoiceGroup() });
      return unison && unison->isAnyParameterChanged();
    }
    case SoundType::Split:
    {
      auto unison = eb->getParameterGroupByID({ "Unison", Application::get().getVGManager()->getCurrentVoiceGroup() });
      return unison && unison->isAnyParameterChanged();
    }
  }
  return false;
}

void DescriptiveLayouts::CurrentVoiceGroupText::onChange(VoiceGroup newSelection)
{
  setValue({ toString(newSelection), 0 });
}

void DescriptiveLayouts::VGIMuted::onChange(const EditBuffer *eb)
{
  auto muteI = eb->findParameterByID({ C15::PID::Voice_Grp_Mute, VoiceGroup::I })->getControlPositionValue() != 0;
  setValue({ muteI ? "\uE0BA" : "", 0 });
}

void DescriptiveLayouts::VGIIMuted::onChange(const EditBuffer *eb)
{
  auto muteII = eb->findParameterByID({ C15::PID::Voice_Grp_Mute, VoiceGroup::II })->getControlPositionValue() != 0;
  setValue({ muteII ? "\uE0BA" : "", 0 });
}

void DescriptiveLayouts::IsSingleSound::onChange(const EditBuffer *eb)
{
  setValue(eb->getType() == SoundType::Single);
}

void DescriptiveLayouts::IsLayerSound::onChange(const EditBuffer *eb)
{
  setValue(eb->getType() == SoundType::Layer);
}

void DescriptiveLayouts::IsSplitSound::onChange(const EditBuffer *eb)
{
  setValue(eb->getType() == SoundType::Split);
}

template <typename... Args> bool anyControlPositionNotZero(const EditBuffer *eb, Args... args)
{
  return ((eb->findParameterByID(args)->getControlPositionValue() != 0) || ...);
}

template <typename... Args> bool anyControlPositionGreaterThanZero(const EditBuffer *eb, Args... args)
{
  return ((eb->findParameterByID(args)->getControlPositionValue() > 0) || ...);
}

void DescriptiveLayouts::AnyLayerCrossFB::onChange(const EditBuffer *eb)
{
  setValue(anyControlPositionNotZero(
      eb, ParameterId(C15::PID::FB_Mix_Comb_Src, VoiceGroup::I), ParameterId(C15::PID::FB_Mix_Comb_Src, VoiceGroup::II),
      ParameterId(C15::PID::FB_Mix_SVF_Src, VoiceGroup::I), ParameterId(C15::PID::FB_Mix_SVF_Src, VoiceGroup::II),
      ParameterId(C15::PID::FB_Mix_FX_Src, VoiceGroup::I), ParameterId(C15::PID::FB_Mix_FX_Src, VoiceGroup::II)));
}

void DescriptiveLayouts::AnyLayerCrossFBFromII::onChange(const EditBuffer *eb)
{
  setValue(anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Comb_Src, VoiceGroup::II },
                                     ParameterId { C15::PID::FB_Mix_SVF_Src, VoiceGroup::II },
                                     ParameterId { C15::PID::FB_Mix_FX_Src, VoiceGroup ::II }));
}

void DescriptiveLayouts::AnyLayerCrossFBFromI::onChange(const EditBuffer *eb)
{
  setValue(anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Comb_Src, VoiceGroup::I },
                                     ParameterId { C15::PID::FB_Mix_SVF_Src, VoiceGroup::I },
                                     ParameterId { C15::PID::FB_Mix_FX_Src, VoiceGroup ::I }));
}

void DescriptiveLayouts::ToFXIUnder100::onChange(const EditBuffer *eb)
{
  setValue(eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II })->getControlPositionValue() < 1.0);
}

void DescriptiveLayouts::ToFXIIUnder100::onChange(const EditBuffer *eb)
{
  setValue(eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I })->getControlPositionValue() < 1.0);
}

bool DescriptiveLayouts::ToFXIOver0::check(const EditBuffer *eb)
{
  return eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II })->getControlPositionValue() > 0;
}

void DescriptiveLayouts::ToFXIOver0::onChange(const EditBuffer *eb)
{
  setValue(check(eb));
}

bool DescriptiveLayouts::ToFXIIOver0::check(const EditBuffer *eb)
{
  return eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I })->getControlPositionValue() > 0;
}

void DescriptiveLayouts::ToFXIIOver0::onChange(const EditBuffer *eb)
{
  setValue(check(eb));
}

bool DescriptiveLayouts::LayerIIFBFromI::check(const EditBuffer *eb)
{
  auto oscFB = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Osc, VoiceGroup::II });

  auto combMix = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Comb, VoiceGroup::II });
  auto combSrc = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Comb_Src, VoiceGroup::II });
  auto comb = combMix && combSrc;

  auto svfMix = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_SVF, VoiceGroup::II });
  auto svfSrc = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_SVF_Src, VoiceGroup::II });
  auto svf = svfMix && svfSrc;

  auto fxMix = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_FX, VoiceGroup::II });
  auto fxSrc = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_FX_Src, VoiceGroup::II });
  auto fx = fxMix && fxSrc;

  auto pfbl = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Lvl, VoiceGroup::II });
  auto oscPM = anyControlPositionNotZero(eb, ParameterId { C15::PID::Osc_A_PM_FB, VoiceGroup::II },
                                         ParameterId { C15::PID::Osc_B_PM_FB, VoiceGroup::II });
  auto shaperFB = anyControlPositionGreaterThanZero(eb, ParameterId(C15::PID::Shp_A_FB_Mix, VoiceGroup::II),
                                                    ParameterId(C15::PID::Shp_B_FB_Mix, VoiceGroup::II));

  auto other = pfbl && (shaperFB || oscPM);
  auto state = comb || svf || fx;
  return (oscFB || state) && other;
}

bool DescriptiveLayouts::LayerIFBFromII::check(const EditBuffer *eb)
{
  auto oscFB = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Osc, VoiceGroup::I });

  auto combMix = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Comb, VoiceGroup::I });
  auto combSrc = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Comb_Src, VoiceGroup::I });
  auto comb = combMix && combSrc;

  auto svfMix = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_SVF, VoiceGroup::I });
  auto svfSrc = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_SVF_Src, VoiceGroup::I });
  auto svf = svfMix && svfSrc;

  auto fxMix = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_FX, VoiceGroup::I });
  auto fxSrc = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto fx = fxMix && fxSrc;

  auto pfbl = anyControlPositionNotZero(eb, ParameterId { C15::PID::FB_Mix_Lvl, VoiceGroup::I });
  auto oscPM = anyControlPositionNotZero(eb, ParameterId { C15::PID::Osc_A_PM_FB, VoiceGroup::I },
                                         ParameterId { C15::PID::Osc_B_PM_FB, VoiceGroup::I });
  auto shaperFB = anyControlPositionGreaterThanZero(eb, ParameterId(C15::PID::Shp_A_FB_Mix, VoiceGroup::I),
                                                    ParameterId(C15::PID::Shp_B_FB_Mix, VoiceGroup::I));

  auto other = pfbl && (shaperFB || oscPM);
  auto state = comb || svf || fx;
  return (oscFB || state) && other;
}

void DescriptiveLayouts::LayerFBState::onChange(const EditBuffer *eb)
{
  if(!LayerIFBFromII::check(eb) && LayerIIFBFromI::check(eb))
  {
    setValue("Layer_FB_B.png");
  }
  else if(LayerIFBFromII::check(eb) && !LayerIIFBFromI::check(eb))
  {
    setValue("Layer_FB_A.png");
  }
  else if(LayerIFBFromII::check(eb) && LayerIIFBFromI::check(eb))
  {
    setValue("Layer_FB_C.png");
  }
  else
  {
    setValue("");
  }
}

void DescriptiveLayouts::LayerFXState::onChange(const EditBuffer *eb)
{
  auto IToII = ToFXIIOver0::check(eb);
  auto IIToI = ToFXIOver0::check(eb);

  if(IToII && !IIToI)
  {
    setValue("Layer_FX_B.png");
  }
  else if(!IToII && IIToI)
  {
    setValue("Layer_FX_A.png");
  }
  else if(IToII && IIToI)
  {
    setValue("Layer_FX_C.png");
  }
  else
  {
    setValue("");
  }
}

void DescriptiveLayouts::LayerFXOffset::onChange(const EditBuffer *eb)
{
  auto IToII = ToFXIIOver0::check(eb);
  auto IIToI = ToFXIOver0::check(eb);

  if(!IIToI && IToII)
  {
    setValue({ 0, 2 });
  }
  else
  {
    setValue({ 0, 0 });
  }
}

DescriptiveLayouts::SplitPointBehaviourIsDefaultWithoutSync::SplitPointBehaviourIsDefaultWithoutSync()
    : EditBufferEvent()
{
  auto sync = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  sync->onChange(sigc::mem_fun(this, &SplitPointBehaviourIsDefaultWithoutSync::onSettingChanged));
}

void DescriptiveLayouts::SplitPointBehaviourIsDefaultWithoutSync::onChange(const EditBuffer *eb)
{
  auto sI = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto sync = Application::get().getSettings()->getSetting<SplitPointSyncParameters>();
  setValue((!sync->get()) && sI->inDefaultSplitBehaviour());
}

void DescriptiveLayouts::SplitPointBehaviourIsDefaultWithoutSync::onSettingChanged(const Setting *s)
{
  onChange(Application::get().getPresetManager()->getEditBuffer());
}

void DescriptiveLayouts::VGIIsMuted::onChange(const EditBuffer *eb)
{
  setValue(eb->findParameterByID({ C15::PID::Voice_Grp_Mute, VoiceGroup::I })->isValueDifferentFrom(0));
}

void DescriptiveLayouts::VGIIIsMuted::onChange(const EditBuffer *eb)
{
  setValue(eb->findParameterByID({ C15::PID::Voice_Grp_Mute, VoiceGroup::II })->isValueDifferentFrom(0));
}

void DescriptiveLayouts::SoundFxMixMasterButtonText::onChange(const EditBuffer *eb)
{
  auto changed = isChanged(eb);
  setValue({ changed ? "FX Mix..*" : "FX Mix..", 0 });
}

bool DescriptiveLayouts::SoundFxMixMasterButtonText::isChanged(const EditBuffer *eb)
{
  return eb->findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global })->isChangedFromLoaded();
}

void DescriptiveLayouts::FX_I_ImageState::onChange(const EditBuffer *eb)
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  setValue(currentVG == VoiceGroup::I ? "FX_I_select.png" : "FX_I_non_select.png");
}

void DescriptiveLayouts::FX_II_ImageState::onChange(const EditBuffer *eb)
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  setValue(currentVG == VoiceGroup::II ? "FX_II_select.png" : "FX_II_non_select.png");
}

void DescriptiveLayouts::LayerToFXPath::onChange(const EditBuffer *eb)
{
  const auto toFX_I = eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  const auto toFX_II = eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
  const auto outmixer_lvl_I = eb->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I });
  const auto outmixer_lvl_II = eb->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::II });

  nltools_assertAlways(outmixer_lvl_I->isBiPolar() == false);
  nltools_assertAlways(outmixer_lvl_II->isBiPolar() == false);

  const auto I_To_FX_II = toFX_I->getControlPositionValue();
  const auto II_To_FX_I = toFX_II->getControlPositionValue();
  const auto I_OutMixer_Level = outmixer_lvl_I->getControlPositionValue();
  const auto II_OutMixer_Level = outmixer_lvl_II->getControlPositionValue();

  auto setResult = [this](const std::string &c)
  {
    if(c == "ERROR")
    {
      setValue(c);
      return;
    }
    if(c == "empty")
    {
      setValue("Layer_To_FX_Empty");
      return;
    }
    const std::string base_string = "Layer_To_FX_";
    const std::string base_suffix = ".png";
    setValue(base_string + c + base_suffix);
  };

  const auto outmix_I_infinite = I_OutMixer_Level == 0;
  const auto outmix_II_infinite = II_OutMixer_Level == 0;
  const auto outmix_I_open = I_OutMixer_Level > 0;
  const auto outmix_II_open = II_OutMixer_Level > 0;
  const auto FX_I_TO_I_ONLY = I_To_FX_II == 0;
  const auto FX_II_TO_II_ONLY = II_To_FX_I == 0;
  const auto FX_I_TO_II_ONLY = I_To_FX_II == 1;
  const auto FX_II_TO_I_ONLY = II_To_FX_I == 1;
  const auto FX_I_TO_BOTH = I_To_FX_II > 0 && I_To_FX_II < 1;
  const auto FX_II_TO_BOTH = II_To_FX_I > 0 && II_To_FX_I < 1;

  if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_BOTH)
    setResult("M");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
    setResult("I");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
    setResult("K");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
    setResult("L");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
    setResult("H");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
    setResult("J");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
    setResult("G");
  else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
    setResult("E");
  else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
    setResult("C");
  else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
    setResult("A");
  else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
    setResult("F");
  else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
    setResult("D");
  else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
    setResult("B");
  else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
    setResult("N");
  else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
    setResult("O");
  else if(outmix_I_infinite && outmix_II_infinite)
    setResult("empty");
  else
    setResult("ERROR");
}

std::string DescriptiveLayouts::LayerToFXPath::getValue()
{
  return std::experimental::any_cast<std::string>(getLastValue());
}

void DescriptiveLayouts::Serial_FX_Imagestate::onChange(const EditBuffer *eb)
{
  auto serialFX = eb->findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global });
  const auto cp = serialFX->getControlPositionValue();
  if(cp < 0)
    setValue("ArrowUp.png");
  else if(cp > 0)
    setValue("ArrowDown.png");
  else
    setValue("Layer_To_FX_Empty.png");
}

void DescriptiveLayouts::Layer_FX_TO_OUT_Imagestate::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string base_string = "Layer_FX_To_Out_";
    const std::string base_suffix = ".png";
    if(s == "empty")
      setValue("Layer_To_FX_empty.png");
    else
      setValue(base_string + s + base_suffix);
  };

  auto partVolumeI = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto partVolumeII = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::II });
  auto partMuteI = eb->findParameterByID({ C15::PID::Voice_Grp_Mute, VoiceGroup::I });
  auto partMuteII = eb->findParameterByID({ C15::PID::Voice_Grp_Mute, VoiceGroup::II });

  nltools_assertAlways(partVolumeI->isBiPolar() == false);
  nltools_assertAlways(partVolumeII->isBiPolar() == false);
  nltools_assertAlways(partMuteI->isBiPolar() == false);
  nltools_assertAlways(partMuteII->isBiPolar() == false);

  if(partVolumeI->getControlPositionValue() > 0
     && (partVolumeII->getControlPositionValue() == 0 || partMuteII->getControlPositionValue() == 1))
    setResult("A");
  else if(partVolumeII->getControlPositionValue() > 0
          && (partVolumeI->getControlPositionValue() == 0 || partMuteI->getControlPositionValue() == 1))
    setResult("B");
  else if(partVolumeI->getControlPositionValue() > 0 && partVolumeII->getControlPositionValue() > 0)
    setResult("C");
  else if(partVolumeI->getControlPositionValue() == 0 && partVolumeII->getControlPositionValue() == 0)
    setResult("empty");
}

void DescriptiveLayouts::Split_FX_TO_OUT_Imagestate::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string base_string = "Layer_FX_To_Out_";
    const std::string base_suffix = ".png";
    if(s == "empty")
      setValue("Layer_FX_To_Out_empty.png");
    else
      setValue(base_string + s + base_suffix);
  };

  auto partVolumeI = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto partVolumeII = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::II });

  nltools_assertAlways(partVolumeI->isBiPolar() == false);
  nltools_assertAlways(partVolumeII->isBiPolar() == false);

  if(partVolumeI->getControlPositionValue() > 0 && partVolumeII->getControlPositionValue() == 0)
    setResult("A");
  else if(partVolumeII->getControlPositionValue() > 0 && partVolumeI->getControlPositionValue() == 0)
    setResult("B");
  else if(partVolumeI->getControlPositionValue() > 0 && partVolumeII->getControlPositionValue() > 0)
    setResult("C");
  else if(partVolumeI->getControlPositionValue() == 0 && partVolumeII->getControlPositionValue() == 0)
    setResult("empty");
}

void DescriptiveLayouts::Split_FX_TO_OUT_Imagestate_flipped::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string base_string = "Layer_FX_To_Out_";
    const std::string base_suffix = "_flipped.png";
    if(s == "empty")
      setValue("Layer_FX_To_Out_empty.png");
    else
      setValue(base_string + s + base_suffix);
  };

  auto partVolumeI = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::I });
  auto partVolumeII = eb->findParameterByID({ C15::PID::Voice_Grp_Volume, VoiceGroup::II });

  nltools_assertAlways(partVolumeI->isBiPolar() == false);
  nltools_assertAlways(partVolumeII->isBiPolar() == false);

  if(partVolumeI->getControlPositionValue() > 0 && partVolumeII->getControlPositionValue() == 0)
    setResult("A");
  else if(partVolumeII->getControlPositionValue() > 0 && partVolumeI->getControlPositionValue() == 0)
    setResult("B");
  else if(partVolumeI->getControlPositionValue() > 0 && partVolumeII->getControlPositionValue() > 0)
    setResult("C");
  else if(partVolumeI->getControlPositionValue() == 0 && partVolumeII->getControlPositionValue() == 0)
    setResult("empty");
}

void DescriptiveLayouts::Split_Arrows_To_FX_L_TO_R_I::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string texture = "Split_FX_FB_L_TO_R.png";
    if(s == "empty")
      setValue("Split_FX_FB_Empty.png");
    else
      setValue(texture);
  };

  auto I_FB_Mixer_Effects_From_II = eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto I_FB_Mixer_Effects = eb->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I });
  auto I_FB_Mixer_Level = eb->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I });
  auto I_OSC_A_PM_FB = eb->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });
  auto I_OSC_B_PM_FB = eb->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I });
  auto I_SHAPER_A_FB_Mix = eb->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I });
  auto I_SHAPER_B_FB_Mix = eb->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I });

  nltools_assertAlways(I_FB_Mixer_Effects_From_II->isBiPolar() == false);
  nltools_assertAlways(I_FB_Mixer_Effects->isBiPolar() == true);
  nltools_assertAlways(I_OSC_A_PM_FB->isBiPolar() == true);
  nltools_assertAlways(I_OSC_B_PM_FB->isBiPolar() == true);
  nltools_assertAlways(I_SHAPER_A_FB_Mix->isBiPolar() == false);
  nltools_assertAlways(I_SHAPER_B_FB_Mix->isBiPolar() == false);

  if(I_FB_Mixer_Effects_From_II->getControlPositionValue() < 1 && I_FB_Mixer_Effects->getControlPositionValue() != 0
     && I_FB_Mixer_Level->getControlPositionValue() > 0
     && (I_OSC_A_PM_FB->getControlPositionValue() != 0 || I_OSC_B_PM_FB->getControlPositionValue() != 0
         || I_SHAPER_A_FB_Mix->getControlPositionValue() > 0 || I_SHAPER_B_FB_Mix->getControlPositionValue() > 0))
  {
    setResult("ok");
  }
  else
  {
    setResult("empty");
  }
}

void DescriptiveLayouts::Split_Arrows_To_FX_L_TO_R_II::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string texture = "Split_FX_FB_L_TO_R.png";
    if(s == "empty")
      setValue("Split_FX_FB_Empty.png");
    else
      setValue(texture);
  };

  auto I_FB_Mixer_Effects_From_II = eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto I_FB_Mixer_Effects = eb->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I });
  auto I_FB_Mixer_Level = eb->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I });
  auto I_OSC_A_PM_FB = eb->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });
  auto I_OSC_B_PM_FB = eb->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I });
  auto I_SHAPER_A_FB_Mix = eb->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I });
  auto I_SHAPER_B_FB_Mix = eb->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I });

  nltools_assertAlways(I_FB_Mixer_Effects_From_II->isBiPolar() == false);
  nltools_assertAlways(I_FB_Mixer_Effects->isBiPolar() == true);
  nltools_assertAlways(I_OSC_A_PM_FB->isBiPolar() == true);
  nltools_assertAlways(I_OSC_B_PM_FB->isBiPolar() == true);
  nltools_assertAlways(I_SHAPER_A_FB_Mix->isBiPolar() == false);
  nltools_assertAlways(I_SHAPER_B_FB_Mix->isBiPolar() == false);

  if(I_FB_Mixer_Effects_From_II->getControlPositionValue() > 0 && I_FB_Mixer_Effects->getControlPositionValue() != 0
     && I_FB_Mixer_Level->getControlPositionValue() > 0
     && (I_OSC_A_PM_FB->getControlPositionValue() != 0 || I_OSC_B_PM_FB->getControlPositionValue() != 0
         || I_SHAPER_A_FB_Mix->getControlPositionValue() > 0 || I_SHAPER_B_FB_Mix->getControlPositionValue() > 0))
  {
    setResult("ok");
  }
  else
  {
    setResult("empty");
  }
}

void DescriptiveLayouts::Split_Arrows_To_FX_R_TO_L_I::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string texture = "Split_FX_FB_R_TO_L.png";
    if(s == "empty")
      setValue("Split_FX_FB_Empty.png");
    else
      setValue(texture);
  };

  auto II_FB_Mixer_Effects_From_II = eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });
  auto II_FB_Mixer_Effects = eb->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::II });
  auto II_FB_Mixer_Level = eb->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::II });
  auto II_OSC_A_PM_FB = eb->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::II });
  auto II_OSC_B_PM_FB = eb->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::II });
  auto II_SHAPER_A_FB_Mix = eb->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::II });
  auto II_SHAPER_B_FB_Mix = eb->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::II });

  nltools_assertAlways(II_FB_Mixer_Effects_From_II->isBiPolar() == false);
  nltools_assertAlways(II_FB_Mixer_Effects->isBiPolar() == true);
  nltools_assertAlways(II_OSC_A_PM_FB->isBiPolar() == true);
  nltools_assertAlways(II_OSC_B_PM_FB->isBiPolar() == true);
  nltools_assertAlways(II_SHAPER_A_FB_Mix->isBiPolar() == false);
  nltools_assertAlways(II_SHAPER_B_FB_Mix->isBiPolar() == false);

  if(II_FB_Mixer_Effects_From_II->getControlPositionValue() < 1 && II_FB_Mixer_Effects->getControlPositionValue() != 0
     && II_FB_Mixer_Level->getControlPositionValue() > 0
     && (II_OSC_A_PM_FB->getControlPositionValue() != 0 || II_OSC_B_PM_FB->getControlPositionValue() != 0
         || II_SHAPER_A_FB_Mix->getControlPositionValue() > 0 || II_SHAPER_B_FB_Mix->getControlPositionValue() > 0))
  {
    setResult("ok");
  }
  else
  {
    setResult("empty");
  }
}

void DescriptiveLayouts::Split_Arrows_To_FX_R_TO_L_II::onChange(const EditBuffer *eb)
{
  auto setResult = [this](const std::string &s)
  {
    const std::string texture = "Split_FX_FB_R_TO_L.png";
    if(s == "empty")
      setValue("Split_FX_FB_Empty.png");
    else
      setValue(texture);
  };

  auto II_FB_Mixer_Effects_From_II = eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });
  auto II_FB_Mixer_Effects = eb->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::II });
  auto II_FB_Mixer_Level = eb->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::II });
  auto II_OSC_A_PM_FB = eb->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::II });
  auto II_OSC_B_PM_FB = eb->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::II });
  auto II_SHAPER_A_FB_Mix = eb->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::II });
  auto II_SHAPER_B_FB_Mix = eb->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::II });

  nltools_assertAlways(II_FB_Mixer_Effects_From_II->isBiPolar() == false);
  nltools_assertAlways(II_FB_Mixer_Effects->isBiPolar() == true);
  nltools_assertAlways(II_OSC_A_PM_FB->isBiPolar() == true);
  nltools_assertAlways(II_OSC_B_PM_FB->isBiPolar() == true);
  nltools_assertAlways(II_SHAPER_A_FB_Mix->isBiPolar() == false);
  nltools_assertAlways(II_SHAPER_B_FB_Mix->isBiPolar() == false);

  if(II_FB_Mixer_Effects_From_II->getControlPositionValue() > 0 && II_FB_Mixer_Effects->getControlPositionValue() != 0
     && II_FB_Mixer_Level->getControlPositionValue() > 0
     && (II_OSC_A_PM_FB->getControlPositionValue() != 0 || II_OSC_B_PM_FB->getControlPositionValue() != 0
         || II_SHAPER_A_FB_Mix->getControlPositionValue() > 0 || II_SHAPER_B_FB_Mix->getControlPositionValue() > 0))
  {
    setResult("ok");
  }
  else
  {
    setResult("empty");
  }
}

void DescriptiveLayouts::InitCurrentVoiceText::onChange(VoiceGroup newSelection)
{
  auto str = nltools::string::concat("Part ", toString(newSelection));
  setValue({str, 0});
}
