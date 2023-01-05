#pragma once
#include <nltools/Types.h>
#include <GroupId.h>
#include "ParameterUseCases.h"
#include "ModParameterUseCases.h"
#include "MacroControlParameterUseCases.h"
#include <memory>
#include <playground.h>
#include <tools/Uuid.h>

class EditBuffer;
class Preset;
class PresetManager;
class HWUI;
class DirectLoadSetting;

class EditBufferUseCases
{
 public:
  explicit EditBufferUseCases(EditBuffer& eb);

  void setParameter(const ParameterId& id, tControlPositionValue cp);
  void setSplits(const ParameterId& id, tControlPositionValue cp, tControlPositionValue otherCp);

  void setModulationSource(MacroControls src, const ParameterId& id);
  void setModulationAmount(tControlPositionValue amount, const ParameterId& id);
  void setModAmountAndValue(const ParameterId& id, tControlPositionValue modAmt, tControlPositionValue val);
  void setModulationLimit(const ParameterId& id, tControlPositionValue modAmt, tControlPositionValue paramVal);

  void selectParameter(const ParameterId& id, bool sendReselectionSignal = false);
  void selectParameter(const Parameter* param, bool sendReselectionSignal = false);

  void convertToLayer(VoiceGroup currentSelectedVoiceGroup);
  void convertToSingle(VoiceGroup partToUse);
  void convertToSplit(VoiceGroup currentSelectedVoiceGroup);
  void convertToDual(SoundType type, VoiceGroup group);

  void initSoundAs(SoundType type, Defaults defaults);

  void initSound(Defaults defaults);
  void initPart(VoiceGroup part, Defaults defaults);

  void mutePart(VoiceGroup part);
  void unmutePart(VoiceGroup part);
  void unmuteBothPartsWithTransactionNameForPart(VoiceGroup part);
  void mutePartUnmuteOtherPart(VoiceGroup part);

  void load(const Uuid& uuid);
  void load(const Preset* preset);
  void load(const Preset* preset, const std::string& transactionName);
  void loadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to);

  void resetScaleGroup();
  void resetCustomScale();

  void unlockAllGroups();
  void lockAllGroups();
  void toggleLock(const std::string& groupName);
  void unlockGroup(ParameterGroup* group);
  void lockGroup(ParameterGroup* group);

  void lockParametersTemporarily(const std::vector<ParameterId>& id);
  void unlockParametersTemporarily(const std::vector<ParameterId>& id);

  std::unique_ptr<ParameterUseCases> getUseCase(const ParameterId& id);
  std::unique_ptr<ModParameterUseCases> getModParamUseCase(const ParameterId& id);
  std::unique_ptr<MacroControlParameterUseCases> getMCUseCase(const ParameterId& id);

  void toggleMute(VoiceGroup part);

  void renamePart(VoiceGroup part, const Glib::ustring& name);

  void undoableLoadAccordingToType(Preset* pPreset, HWUI* hwui);

  void randomize(double amount);
  void randomizePart(VoiceGroup part, double amount);

  void setModulationSourceOfAll(MacroControls controls);

  void selectLastSelectedMacroControlParameter();

  void copyFX(VoiceGroup from, VoiceGroup to);

 private:
  VoiceGroup invert(VoiceGroup vg);
  [[nodiscard]] PresetManager* getPresetManager() const;

  EditBuffer& m_editBuffer;
};