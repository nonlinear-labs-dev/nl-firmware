#pragma once

#include "BaseUnitPlayMode.h"

class BaseUnitEditMode : public BaseUnitPlayMode
{
 private:
  typedef BaseUnitPlayMode super;

 protected:
  void toggleTouchBehaviour() override;

 private:
  void toggleParameterEditModeRibbonBehaviour();
};
