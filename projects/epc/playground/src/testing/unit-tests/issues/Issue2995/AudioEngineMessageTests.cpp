#include <testing/TestHelper.h>
#include "CompileTimeOptions.h"
#include <presets/Bank.h>
#include <presets/Preset.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "a new split-preset will have different values in preset-msg for part-volume")
{
  auto eb = TestHelper::getEditBuffer();

  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.convertToSplit(VoiceGroup::I);

  auto partVol_I = eb->findParameterByID({C15::PID::Voice_Grp_Volume, VoiceGroup::I});
  auto partVol_II = eb->findParameterByID({C15::PID::Voice_Grp_Volume, VoiceGroup::II});

  auto setParameterValue = [](Parameter* p, double value) {
    ParameterUseCases useCase(p);
    useCase.setControlPosition(value);
  };

  setParameterValue(partVol_I, 0.187);
  setParameterValue(partVol_II, 0.420);

  auto desc = C15::ParameterList[C15::PID::Voice_Grp_Volume];
  const auto arrIndex = desc.m_param.m_index;
  auto message = AudioEngineProxy::createSplitEditBufferMessage(*eb);
  CHECK(message.m_polyphonicModulateables[0][arrIndex].m_controlPosition == Approx(0.187));
  CHECK(message.m_polyphonicModulateables[1][arrIndex].m_controlPosition == Approx(0.420));
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "an older split-preset will have different values in preset-msg for part-volume")
{
  auto pm = app->getPresetManager();
  auto settings = app->getSettings();
  PresetManagerUseCases uc(*pm, *settings);
  FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/Fuxi_Split.xml", false);
  auto bank = uc.importBankFromStream(stream, 0, 0, "fuxis-split", Serializer::MockProgress);
  auto preset = bank->getPresetAt(0);

  auto eb = TestHelper::getEditBuffer();

  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.load(preset);

  auto partVol_I = eb->findParameterByID({C15::PID::Voice_Grp_Volume, VoiceGroup::I});
  auto partVol_II = eb->findParameterByID({C15::PID::Voice_Grp_Volume, VoiceGroup::II});

  CHECK(partVol_I->getControlPositionValue() == Approx(0.374));
  CHECK(partVol_II->getControlPositionValue() == Approx(0.48999999999999999));

  auto desc = C15::ParameterList[C15::PID::Voice_Grp_Volume];
  const auto arrIndex = desc.m_param.m_index;
  auto message = AudioEngineProxy::createSplitEditBufferMessage(*eb);
  CHECK(message.m_polyphonicModulateables[0][arrIndex].m_controlPosition == Approx(0.374));
  CHECK(message.m_polyphonicModulateables[1][arrIndex].m_controlPosition == Approx(0.48999999999999999));
}