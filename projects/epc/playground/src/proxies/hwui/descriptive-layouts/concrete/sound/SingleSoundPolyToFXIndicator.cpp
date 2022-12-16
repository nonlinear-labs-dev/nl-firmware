#include "SingleSoundPolyToFXIndicator.h"
#include "proxies/hwui/controls/PNGControl.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"

SingleSoundPolyToFXIndicator::SingleSoundPolyToFXIndicator(const Point& p)
    : ControlWithChildren({ p.getX(), p.getY(), 50, 50 })
{
  polytoFXI = addControl(new PNGControl({ 0, 12, 11, 7 }, "Single_FX_I_Out.png"));
  polytoFXII = addControl(new PNGControl({ 0, 4, 11, 7 }, "Single_FX_II_out.png"));

  FXI = addControl(new PNGControl({ 17, 0, 9, 9 }, "Single_FX_I.png"));
  FXII = addControl(new PNGControl({ 17, 15, 9, 9 }, "Single_FX_II.png"));

  fxIToOut = addControl(new PNGControl({ 28, 12, 11, 7 }, "Single_FX_II_out_no_arrow.png"));
  fxIIToOut = addControl(new PNGControl({ 28, 6, 11, 7 }, "Single_FX_I_Out_no_arrow.png"));
  arrowFromFX = addControl(new PNGControl({ 39, 10, 3, 5 }, "Single_FX_Arrow.png"));

  auto& eb = *Application::get().getPresetManager()->getEditBuffer();
  auto out_mix_to_fx_I = eb.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto master_fx_mix = eb.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global });
  auto master_serial_fx = eb.findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global });

  auto& vgManager = *Application::get().getVGManager();
  con1 = out_mix_to_fx_I->onParameterChanged(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::onToFXI));
  con2 = master_fx_mix->onParameterChanged(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::onMasterFXMix));
  con3 = vgManager.onCurrentVoiceGroupChanged(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::onVoiceGroupChanged));
  con4 = master_serial_fx->onParameterChanged(sigc::mem_fun(this, &SingleSoundPolyToFXIndicator::onSerialFX));
}

void SingleSoundPolyToFXIndicator::onVoiceGroupChanged(VoiceGroup vg)
{
  FXI->setHighlight(vg == VoiceGroup::I);
  FXII->setHighlight(vg == VoiceGroup::II);
}

void SingleSoundPolyToFXIndicator::onToFXI(const Parameter* param)
{
  auto cp = param->getControlPositionValue();
  polytoFXI->setVisible(cp != 0);
  polytoFXII->setVisible(cp != 1);
}

void SingleSoundPolyToFXIndicator::onMasterFXMix(const Parameter* param)
{
  auto cp = param->getControlPositionValue();
  fxIToOut->setVisible(cp != 0);
  fxIIToOut->setVisible(cp != 1);
  arrowFromFX->setVisible(fxIToOut->isVisible() || fxIIToOut->isVisible());
}

void SingleSoundPolyToFXIndicator::onSerialFX(const Parameter* param)
{
  auto cp = param->getControlPositionValue();

  remove(arrowSerial);

  if(cp > 0)
    arrowSerial = addControl(new PNGControl({ 19, 10, 5, 4 }, "ArrowDown.png"));
  else if(cp < 0)
    arrowSerial = addControl(new PNGControl({ 19, 10, 5, 4 }, "ArrowUp.png"));
}

SingleSoundFBFXIndicator::SingleSoundFBFXIndicator(const Point& p)
    : ControlWithChildren({ p.getX(), p.getY(), 44, 28 })
{
  Application::get().getVGManager()->onCurrentVoiceGroupChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundFBFXIndicator::bruteForce)));

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vgIParams = { C15::PID::FB_Mix_FX,   C15::PID::FB_Mix_Lvl,   C15::PID::Osc_A_PM_FB,
                     C15::PID::Osc_B_PM_FB, C15::PID::Shp_A_FB_Mix, C15::PID::Shp_B_FB_Mix };

  for(auto pid : vgIParams)
  {
    eb->findParameterByID({ pid, VoiceGroup::I})->onParameterChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundFBFXIndicator::bruteForce)));
  }

  auto globalParams = {C15::PID::Master_Serial_FX};

  for(auto pid: globalParams)
  {
    eb->findParameterByID({pid, VoiceGroup::Global})->onParameterChanged(sigc::hide(sigc::mem_fun(this, &SingleSoundFBFXIndicator::bruteForce)));
  }
}

void SingleSoundFBFXIndicator::bruteForce()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  const auto isLabel1Visible = isControl1Visible();
  const auto masterSerialFXCP = eb->findParameterByID({C15::PID::Master_Serial_FX, VoiceGroup::Global})->getControlPositionValue();

  remove(label1);
  remove(FXI);
  remove(FXII);
  remove(serial);
  remove(fbToPoly);

  if(isLabel1Visible)
  {
    label1 = addControl(new PNGControl({0, (getHeight() / 2) - 5, 8, 5}, "FB_Label.png"));
    if(currentVG == VoiceGroup::I)
    {
      FXI = addControl(new PNGControl({8, 0, 16, 7}, "FX_I_select.png"));
      FXII = addControl(new PNGControl({8, 16, 16, 7}, "FX_II_non_select.png"));
    }
    else
    {
      FXI = addControl(new PNGControl({8, 0, 16, 7}, "FX_I_non_select.png"));
      FXII = addControl(new PNGControl({8, 16, 16, 7}, "FX_II_select.png"));
    }

    if(masterSerialFXCP > 0)
    {
      serial = addControl(new PNGControl({12, 7, 7, 9}, "ArrowDown.png"));
    }
    else if(masterSerialFXCP < 0)
    {
      serial = addControl(new PNGControl({12, 7, 7, 9}, "ArrowUp.png"));
    }

    auto fb_mixer_fx_src = eb->findParameterByID({C15::PID::FB_Mix_FX_Src, VoiceGroup::I})->getControlPositionValue();

    if(fb_mixer_fx_src > 0 && fb_mixer_fx_src < 1)
    {
      fbToPoly = addControl(new PNGControl({24, 2, 16, 16}, "Single_2_Arrows_Right_To_Left.png"));
    }
    else if(fb_mixer_fx_src == 0)
    {
      fbToPoly = addControl(new PNGControl({24, 2, 16, 16}, "Single_Upper_Arrow_Right_To_Left.png"));
    }
    else if(fb_mixer_fx_src == 1)
    {
      fbToPoly = addControl(new PNGControl({24, 2, 16, 16}, "Single_Lower_Arrow_Right_To_Left.png"));
    }
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
