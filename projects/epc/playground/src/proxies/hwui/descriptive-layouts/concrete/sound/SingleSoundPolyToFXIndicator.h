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
  void bruteForce();

  Control* polyToFx = nullptr;
  Control* FXI = nullptr;
  Control* FXII = nullptr;
  Control* fxToOut = nullptr;
  Control* serial = nullptr;

  sigc::connection con1, con2, con3, con4;
};

class SingleSoundFBFXIndicator : public ControlWithChildren
{
 public:
  explicit SingleSoundFBFXIndicator(const Point& p);

 private:
  void bruteForce();
  bool isControl1Visible() const;

  Control* label1 = nullptr;
  Control* FXI = nullptr;
  Control* FXII = nullptr;
  Control* serial = nullptr;
  Control* fbToPoly = nullptr;
};