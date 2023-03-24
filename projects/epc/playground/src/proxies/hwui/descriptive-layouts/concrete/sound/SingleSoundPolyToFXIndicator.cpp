#include "SingleSoundPolyToFXIndicator.h"
#include "proxies/hwui/controls/PNGControl.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"

SingleSoundPolyToFXIndicator::SingleSoundPolyToFXIndicator(const Point& p)
    : ControlWithChildren({ p.getX(), p.getY(), 60, 50 })
{
  auto& eb = *Application::get().getPresetManager()->getEditBuffer();
  auto params = { C15::PID::Master_FX_Mix, C15::PID::Master_Serial_FX };

  for(auto pa : params)
  {
    eb.findParameterByID({ pa, VoiceGroup::Global })
        ->onParameterChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::bruteForce)));
  }

  auto vgParams = { C15::PID::Out_Mix_To_FX };
  for(auto pa : vgParams)
  {
    eb.findParameterByID({ pa, VoiceGroup::I })
        ->onParameterChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::bruteForce)));
  }

  Application::get().getVGManager()->onCurrentVoiceGroupChanged(
      sigc::hide(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::bruteForce)));
}

void SingleSoundPolyToFXIndicator::bruteForce()
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto& eb = *Application::get().getPresetManager()->getEditBuffer();

  remove(polyToFx);
  remove(fxToOut);
  remove(FXI);
  remove(FXII);
  remove(serial);

  if(currentVG == VoiceGroup::I)
  {
    FXI = addControl(new PNGControl({ 20, 0, 16, 7 }, "FX_I_select.png"));
    FXII = addControl(new PNGControl({ 20, 16, 16, 7 }, "FX_II_non_select.png"));
  }
  else
  {
    FXI = addControl(new PNGControl({ 20, 0, 16, 7 }, "FX_I_non_select.png"));
    FXII = addControl(new PNGControl({ 20, 16, 16, 7 }, "FX_II_select.png"));
  }

  auto out_mix_to_fx_cp = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I })->getControlPositionValue();

  if(out_mix_to_fx_cp > 0 && out_mix_to_fx_cp < 1)
  {
    polyToFx = addControl(new PNGControl({ 0, 0, 20, 23 }, "Single_2_Arrows_Right_Side.png"));
  }
  else if(out_mix_to_fx_cp == 0)
  {
    polyToFx = addControl(new PNGControl({ 0, 0, 20, 23 }, "Single_Upper_Arrow_Right_Side.png"));
  }
  else if(out_mix_to_fx_cp == 1)
  {
    polyToFx = addControl(new PNGControl({ 0, 0, 20, 23 }, "Single_Lower_Arrow_Right_Side.png"));
  }

  auto master_serial_cp
      = eb.findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global })->getControlPositionValue();

  if(master_serial_cp > 0)
  {
    serial = addControl(new PNGControl({ 24, 7, 7, 9 }, "ArrowDown.png"));
  }
  else if(master_serial_cp < 0)
  {
    serial = addControl(new PNGControl({ 24, 7, 7, 9 }, "ArrowUp.png"));
  }

  auto master_fx_mix_cp
      = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global })->getControlPositionValue();

  if(master_fx_mix_cp > 0 && master_fx_mix_cp < 1)
  {
    fxToOut = addControl(new PNGControl({ 36, 2, 20, 19 }, "Single_2_Arrows_Right_To_Left.png"));
  }
  else if(master_fx_mix_cp == 0)
  {
    fxToOut = addControl(new PNGControl({ 36, 2, 20, 19 }, "Single_Upper_Arrow_Right_To_Left.png"));
  }
  else if(master_fx_mix_cp == 1)
  {
    fxToOut = addControl(new PNGControl({ 36, 2, 20, 19 }, "Single_Lower_Arrow_Right_To_Left.png"));
  }

  for(const auto& c : getControls<PNGControl>())
  {
    c->useImageColors(false);
    c->setColor(FrameBufferColors::C255);
  }
}

SingleSoundFBFXIndicator::SingleSoundFBFXIndicator(const Point& p)
    : ControlWithChildren({ p.getX(), p.getY(), 46, 28 })
{
  Application::get().getVGManager()->onCurrentVoiceGroupChanged(
      sigc::hide(sigc::mem_fun(this, &SingleSoundFBFXIndicator::bruteForce)));

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vgIParams = { C15::PID::FB_Mix_FX,   C15::PID::FB_Mix_Lvl,   C15::PID::Osc_A_PM_FB,
                     C15::PID::Osc_B_PM_FB, C15::PID::Shp_A_FB_Mix, C15::PID::Shp_B_FB_Mix };

  for(auto pid : vgIParams)
  {
    eb->findParameterByID({ pid, VoiceGroup::I })
        ->onParameterChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundFBFXIndicator::bruteForce)));
  }

  auto globalParams = { C15::PID::Master_Serial_FX };

  for(auto pid : globalParams)
  {
    eb->findParameterByID({ pid, VoiceGroup::Global })
        ->onParameterChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundFBFXIndicator::bruteForce)));
  }
}

void SingleSoundFBFXIndicator::bruteForce()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  const auto isLabel1Visible = isControl1Visible();
  const auto masterSerialFXCP
      = eb->findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global })->getControlPositionValue();

  remove(label1);
  remove(FXI);
  remove(FXII);
  remove(serial);
  remove(fbToPoly);

  if(isLabel1Visible)
  {
    if(currentVG == VoiceGroup::I)
    {
      FXI = addControl(new PNGControl({ 10, 0, 16, 7 }, "FX_I_select.png"));
      FXII = addControl(new PNGControl({ 10, 16, 16, 7 }, "FX_II_non_select.png"));
    }
    else
    {
      FXI = addControl(new PNGControl({ 10, 0, 16, 7 }, "FX_I_non_select.png"));
      FXII = addControl(new PNGControl({ 10, 16, 16, 7 }, "FX_II_select.png"));
    }

    if(masterSerialFXCP > 0)
    {
      serial = addControl(new PNGControl({ 14, 7, 7, 9 }, "ArrowDown.png"));
    }
    else if(masterSerialFXCP < 0)
    {
      serial = addControl(new PNGControl({ 14, 7, 7, 9 }, "ArrowUp.png"));
    }

    auto fb_mixer_fx_src = eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I })->getControlPositionValue();

    if(fb_mixer_fx_src > 0 && fb_mixer_fx_src < 1)
    {
      fbToPoly = addControl(new PNGControl({ 26, 2, 16, 16 }, "Single_2_Arrows_Right_To_Left.png"));
    }
    else if(fb_mixer_fx_src == 0)
    {
      fbToPoly = addControl(new PNGControl({ 26, 2, 16, 16 }, "Single_Upper_Arrow_Right_To_Left.png"));
    }
    else if(fb_mixer_fx_src == 1)
    {
      fbToPoly = addControl(new PNGControl({ 26, 2, 16, 16 }, "Single_Lower_Arrow_Right_To_Left.png"));
    }

    label1 = addControl(new PNGControl({ getWidth() - 9, 0, 8, 5 }, "FB_Label.png"));
  }

  for(const auto& c : getControls<PNGControl>())
  {
    c->useImageColors(false);
    c->setTransparent(true);
    c->setColor(FrameBufferColors::C255);
  }
}

bool SingleSoundFBFXIndicator::isControl1Visible() const
{
  auto& eb = *Application::get().getPresetManager()->getEditBuffer();
  auto state = eb.findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I })->getControlPositionValue() != 0
      && eb.findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I })->getControlPositionValue() > 0
      && (eb.findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I })->getControlPositionValue() != 0
          || eb.findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I })->getControlPositionValue() != 0
          || eb.findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I })->getControlPositionValue() > 0
          || eb.findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I })->getControlPositionValue() > 0);
  return state;
}
