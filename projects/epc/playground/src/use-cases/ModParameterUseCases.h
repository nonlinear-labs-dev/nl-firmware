#pragma once
#include <ParameterId.h>
#include <playground.h>
#include <use-cases/ParameterUseCases.h>

class EditBuffer;
class ModulateableParameter;

class ModParameterUseCases : public ParameterUseCases
{
 public:
  explicit ModParameterUseCases(ModulateableParameter* param);

  void recallMCPos();
  void recallMCSource();
  void recallMCAmount();

  void undoRecallMCSel(MacroControls& controls);
  void undoRecallMCAmt(tControlPositionValue mcAmount);
  void undoRecallMCPos(tControlPositionValue mcPos);

  void incMCSelection(int incs);
  void incModAmount(int incs, bool fine);

  void selectModSource(MacroControls mc);

  void removeModSource();
  void selectModSourceAndSelectTargetParameter(MacroControls mc);
  void setModulationAmount(tControlPositionValue amt);

  void incUpperModulationBound(int inc, bool fine);
  void incLowerModulationBound(int inc, bool fine);

 private:
  void setModulationLimit(tControlPositionValue amt, tControlPositionValue cp);

  ModulateableParameter* m_modParam = nullptr;
};
