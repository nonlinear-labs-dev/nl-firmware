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

class SingleSoundFBFXIndicator : public ControlWithChildren
{
 public:
  explicit SingleSoundFBFXIndicator(const Point& p);

 private:
  void onFromFXChanged(const Parameter* p);

  void onVoiceGroupChanged(VoiceGroup vg);

  Control* arrowHead;
  Control* arrowFromI;
  Control* arrowFromII;
  Control* FXI;
  Control* FXII;
};