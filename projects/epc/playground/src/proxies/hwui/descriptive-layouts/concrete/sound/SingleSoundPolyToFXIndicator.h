#pragma once
#include <proxies/hwui/controls/Control.h>
#include "proxies/hwui/controls/ControlOwner.h"
#include "proxies/hwui/controls/ControlWithChildren.h"
#include "nltools/Types.h"

class Parameter;

class SingleSoundPolyToFXIndicator : public ControlWithChildren
{
 public:
  explicit SingleSoundPolyToFXIndicator(const Point& p);

 private:
  Control* polytoFXI;
  Control* polytoFXII;
  Control* FXI;
  Control* FXII;
  Control* fxIToOut;
  Control* fxIIToOut;
  Control* arrowFromFX;
  Control* arrowSerial;

  void onVoiceGroupChanged(VoiceGroup vg);
  void onToFXI(const Parameter* param);
  void onMasterFXMix(const Parameter* param);
  void onSerialFX(const Parameter* param);

  sigc::connection con1, con2, con3, con4;
};