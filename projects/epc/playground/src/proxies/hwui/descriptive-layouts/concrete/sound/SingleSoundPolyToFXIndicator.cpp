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