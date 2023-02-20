#include "ParameterPresenterRules.h"
#include "parameter_declarations.h"
#include "nltools/Types.h"
#include "parameters/ParameterFactory.h"
#include <presets/Preset.h>
#include <presets/PresetParameter.h>
#include <presets/EditBuffer.h>
#include <parameters/Parameter.h>

bool ParameterPresenterRules::isPresetFXUnused_I(Preset *p)
{
  auto out_mix_lvl = p->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I }, true)->getValue();
  auto out_mix_to_fx = p->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I }, true)->getValue();

  auto master_serial_fx = p->findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global }, true)->getValue();
  auto master_fx_mix = p->findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global }, true)->getValue();

  auto fb_mix_lvl = p->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I }, true)->getValue();
  auto fb_mix_fx = p->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I }, true)->getValue();
  auto fb_mix_fx_src = p->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I }, true)->getValue();

  auto osc_a_pm_fb = p->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I }, true)->getValue();
  auto shp_a_fb_mix = p->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I }, true)->getValue();
  auto osc_b_pm_fb = p->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I }, true)->getValue();
  auto shp_b_fb_mix = p->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I }, true)->getValue();

  const auto out_mix = out_mix_lvl > 0 && out_mix_to_fx < 1;
  const auto fx_input = out_mix || master_serial_fx < 0;
  const auto fx_output = master_fx_mix < 1 || master_serial_fx > 0;
  const auto fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src < 1
      && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
  const auto fx_used = fx_input && (fx_output || fx_feedback);
  return !fx_used;
}

bool ParameterPresenterRules::isPresetFXUnused_II(Preset *p)
{
  auto out_mix_lvl = p->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I }, true)->getValue();
  auto out_mix_to_fx = p->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I }, true)->getValue();

  auto master_serial_fx = p->findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global }, true)->getValue();
  auto master_fx_mix = p->findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global }, true)->getValue();

  auto fb_mix_lvl = p->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I }, true)->getValue();
  auto fb_mix_fx = p->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I }, true)->getValue();
  auto fb_mix_fx_src = p->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I }, true)->getValue();

  auto osc_a_pm_fb = p->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I }, true)->getValue();
  auto shp_a_fb_mix = p->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I }, true)->getValue();
  auto osc_b_pm_fb = p->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I }, true)->getValue();
  auto shp_b_fb_mix = p->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I }, true)->getValue();

  const auto out_mix = out_mix_lvl > 0 && out_mix_to_fx > 0;
  const auto fx_input = out_mix || master_serial_fx > 0;
  const auto fx_output = master_fx_mix > 0 || master_serial_fx < 0;
  const auto fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src > 0
      && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
  const auto fx_used = fx_input && (fx_output || fx_feedback);
  return !fx_used;
}

bool ParameterPresenterRules::isSingleFXIUnused(EditBuffer &eb)
{
  auto out_mix_lvl = eb.findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I })->getControlPositionValue();
  auto out_mix_to_fx = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I })->getControlPositionValue();

  auto master_serial_fx
      = eb.findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global })->getControlPositionValue();
  auto master_fx_mix = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global })->getControlPositionValue();

  auto fb_mix_lvl = eb.findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I })->getControlPositionValue();
  auto fb_mix_fx = eb.findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I })->getControlPositionValue();
  auto fb_mix_fx_src = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I })->getControlPositionValue();

  auto osc_a_pm_fb = eb.findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I })->getControlPositionValue();
  auto shp_a_fb_mix = eb.findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I })->getControlPositionValue();
  auto osc_b_pm_fb = eb.findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I })->getControlPositionValue();
  auto shp_b_fb_mix = eb.findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I })->getControlPositionValue();

  const auto out_mix = out_mix_lvl > 0 && out_mix_to_fx < 1;
  const auto fx_input = out_mix || master_serial_fx < 0;
  const auto fx_output = master_fx_mix < 1 || master_serial_fx > 0;
  const auto fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src < 1
      && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
  const auto fx_used = fx_input && (fx_output || fx_feedback);
  return !fx_used;
}

bool ParameterPresenterRules::isSingleFXIIUnused(EditBuffer &eb)
{
  auto out_mix_lvl = eb.findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I })->getControlPositionValue();
  auto out_mix_to_fx = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I })->getControlPositionValue();

  auto master_serial_fx
      = eb.findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global })->getControlPositionValue();
  auto master_fx_mix = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global })->getControlPositionValue();

  auto fb_mix_lvl = eb.findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I })->getControlPositionValue();
  auto fb_mix_fx = eb.findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I })->getControlPositionValue();
  auto fb_mix_fx_src = eb.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I })->getControlPositionValue();

  auto osc_a_pm_fb = eb.findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I })->getControlPositionValue();
  auto shp_a_fb_mix = eb.findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I })->getControlPositionValue();
  auto osc_b_pm_fb = eb.findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I })->getControlPositionValue();
  auto shp_b_fb_mix = eb.findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I })->getControlPositionValue();

  const auto out_mix = out_mix_lvl > 0 && out_mix_to_fx > 0;
  const auto fx_input = out_mix || master_serial_fx > 0;
  const auto fx_output = master_fx_mix > 0 || master_serial_fx < 0;
  const auto fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src > 0
      && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
  const auto fx_used = fx_input && (fx_output || fx_feedback);
  return !fx_used;
}

bool ParameterPresenterRules::allowToggling(const Parameter *selected, const EditBuffer *editBuffer)
{
  if(selected == nullptr)
    return false;

  if(selected->getVoiceGroup() == VoiceGroup::Global)
  {
    return ParameterFactory::isMacroControl(selected->getID());
  }

  if(editBuffer->getType() == SoundType::Single)
  {
    const auto type = selected->getType();
    if(type == C15::Descriptors::ParameterType::Monophonic_Modulateable
       || type == C15::Descriptors::ParameterType::Monophonic_Unmodulateable)
    {
      return true;
    }
    return false;
  }

  auto layerAndGroupAllowToggling
      = ((editBuffer->getType() == SoundType::Layer)
         && (!ParameterFactory::isMonoParameter(selected) && !ParameterFactory::isUnisonParameter(selected)))
      || (editBuffer->getType() != SoundType::Layer);

  if(!selected->isDisabled())
    return layerAndGroupAllowToggling;
  else
    return false;
}

bool ParameterPresenterRules::isLayerPartMuted(VoiceGroup vg, const EditBuffer *eb)
{
  if(auto mute = eb->findParameterByID({ C15::PID::Voice_Grp_Mute, vg }))
  {
    return mute->getControlPositionValue() > 0 && eb->getType() == SoundType::Layer;
  }
  return false;
}
