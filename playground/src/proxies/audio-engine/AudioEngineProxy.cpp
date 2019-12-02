#include "AudioEngineProxy.h"
#include <device-settings/DebugLevel.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <parameters/AftertouchParameter.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/PedalParameter.h>
#include <parameters/PitchbendParameter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/ScaleParameter.h>
#include <parameters/mono-mode-parameters/MonoParameter.h>
#include <parameters/voice-group-master-group/VoiceGroupMasterParameter.h>
#include <groups/HardwareSourcesGroup.h>
#include <groups/MasterGroup.h>
#include <groups/ScaleGroup.h>

AudioEngineProxy::AudioEngineProxy()
{
  using namespace nltools::msg;
  onConnectionEstablished(EndPoint::AudioEngine, sigc::mem_fun(this, &AudioEngineProxy::sendEditBuffer));
}

void AudioEngineProxy::toggleSuppressParameterChanges(UNDO::Transaction *transaction)
{
  transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
    if(std::exchange(m_suppressParamChanges, !m_suppressParamChanges))
      sendEditBuffer();
  });
}

template <typename tMsg> void fillMessageWithGlobalParams(tMsg &msg, EditBuffer *editBuffer)
{
  size_t hwSource = 0;
  size_t globalParams = 0;
  size_t mc = 0;
  size_t modR = 0;

  for(auto &g : editBuffer->getParameterGroups(VoiceGroup::Global))
  {
    for(auto p : g->getParameters())
    {
      auto isHardwareSource = dynamic_cast<HardwareSourcesGroup *>(p->getParentGroup()) != nullptr;
      auto isMaster = dynamic_cast<MasterGroup *>(p->getParentGroup()) != nullptr;
      auto isScale = dynamic_cast<ScaleGroup *>(p->getParentGroup()) != nullptr;

      if(isHardwareSource)
      {
        auto &pItem = msg.hwsources[hwSource++];
        pItem.id = p->getID().getNumber();
        pItem.controlPosition = p->getControlPositionValue();
        pItem.locked = p->isLocked();
      }
      else if(isMaster || isScale)
      {
        auto &pItem = msg.globalparams[globalParams++];
        pItem.id = p->getID().getNumber();
        pItem.controlPosition = p->getControlPositionValue();
        pItem.locked = p->isLocked();
      }
      else if(auto mcParameter = dynamic_cast<MacroControlParameter *>(p))
      {
        auto &macro = msg.macros[mc++];
        macro.id = mcParameter->getID().getNumber();
        macro.controlPosition = mcParameter->getControlPositionValue();
        macro.locked = mcParameter->isLocked();
      }
      else if(auto hwAmounts = dynamic_cast<ModulationRoutingParameter *>(p))
      {
        auto &hwAmount = msg.hwamounts[modR++];
        hwAmount.id = hwAmounts->getID().getNumber();
        hwAmount.controlPosition = hwAmounts->getControlPositionValue();
        hwAmount.locked = hwAmounts->isLocked();
      }
    }
  }

  nltools_assertAlways(msg.globalparams.size() == globalParams);
  nltools_assertAlways(msg.hwsources.size() == hwSource);
  nltools_assertAlways(msg.macros.size() == mc);
  nltools_assertAlways(msg.hwamounts.size() == modR);
}

template <typename tMsg> void insertMockedParameters(tMsg &msg, size_t &unmod, size_t &mod)
{
  using MockParam = std::pair<int, double>;
  using MockParams = std::vector<MockParam>;

  for(auto p :
      MockParams{ { 328, 0.0 }, { 330, 0.0 }, { 332, 0.0 }, { 334, 0.0 }, { 336, 0.0 }, { 338, 0.0 }, { 340, 0.0 } })
  {
    auto &item = msg.unmodulateables[unmod++];
    item.id = p.first;
    item.controlPosition = p.second;
  }

  for(auto p : MockParams{ { 346, 0.0 },
                           { 348, 0.5 },
                           { 350, 0.5 },
                           { 352, 0.5 },
                           { 354, 0.5 },
                           { 389, 0.0 },
                           { 342, 0.0 },
                           { 344, 0.0 } })
  {
    auto &item = msg.modulateables[mod++];
    item.id = p.first;
    item.controlPosition = p.second;
    item.modulationAmount = 0.0;
    item.mc = MacroControls::NONE;
  }
}

template <typename tMsg> void insertMockedParameters(tMsg &msg, VoiceGroup vg, size_t &unmod, size_t &mod)
{
  using MockParam = std::pair<int, double>;
  using MockParams = std::vector<MockParam>;

  const auto index = static_cast<int>(vg);

  for(auto p :
      MockParams{ { 328, 0.0 }, { 330, 0.0 }, { 332, 0.0 }, { 334, 0.0 }, { 336, 0.0 }, { 338, 0.0 }, { 340, 0.0 } })
  {
    auto &item = msg.unmodulateables[index][unmod++];
    item.id = p.first;
    item.controlPosition = p.second;
  }

  for(auto p : MockParams{ { 346, 0.0 },
                           { 348, 0.5 },
                           { 350, 0.5 },
                           { 352, 0.5 },
                           { 354, 0.5 },
                           { 389, 0.0 },
                           { 342, 0.0 },
                           { 344, 0.0 } })
  {
    auto &item = msg.modulateables[index][mod++];
    item.id = p.first;
    item.controlPosition = p.second;
    item.modulationAmount = 0.0;
    item.mc = MacroControls::NONE;
  }
}

template <typename tParameterType, typename tParameterArray>
void forEachParameterInGroup(EditBuffer *eb, const GroupId &group, tParameterArray &array, size_t &index)
{
  for(auto &p : eb->getParameterGroupByID(group)->getParameters())
  {
    if(auto param = dynamic_cast<tParameterType *>(p))
    {
      auto &msgParam = array[index++];
      msgParam.controlPosition = param->getControlPositionValue();
      msgParam.id = param->getID().getNumber();
      msgParam.locked = param->isLocked();
    }
  }
}

nltools::msg::SinglePresetMessage AudioEngineProxy::createSingleEditBufferMessage()
{
  nltools::msg::SinglePresetMessage msg{};
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  fillMessageWithGlobalParams(msg, editBuffer);

  size_t mc = 0;
  size_t modR = 0;
  size_t modP = 0;
  size_t unMod = 0;

  insertMockedParameters(msg, unMod, modP);
  nltools_assertAlways(unMod == 7);
  nltools_assertAlways(modP == 8);

  forEachParameterInGroup<MacroControlParameter>(editBuffer, { "MCs", VoiceGroup::Global }, msg.macros, mc);
  forEachParameterInGroup<ModulationRoutingParameter>(editBuffer, { "MCM", VoiceGroup::Global }, msg.hwamounts, modR);

  for(auto &g : editBuffer->getParameterGroups(VoiceGroup::I))
  {
    for(auto p : g->getParameters())
    {
      if(auto modParam = dynamic_cast<ModulateableParameter *>(p))
      {
        auto &mod = msg.modulateables[modP++];
        mod.id = modParam->getID().getNumber();
        mod.controlPosition = modParam->getControlPositionValue();
        mod.modulationAmount = modParam->getModulationAmount();
        mod.mc = modParam->getModulationSource();
      }
      else
      {
        if(p->getID().getNumber() == 249)
        {
          auto &unisonVoices = msg.unisonVoices;
          unisonVoices.id = 249;
          unisonVoices.controlPosition = p->getControlPositionValue();
          unisonVoices.locked = p->isLocked();
        }
        else
        {
          auto &unModulateable = msg.unmodulateables[unMod++];
          unModulateable.id = p->getID().getNumber();
          unModulateable.controlPosition = p->getControlPositionValue();
          unModulateable.locked = p->isLocked();
        }
      }
    }
  }

  nltools_assertAlways(msg.modulateables.size() == modP);
  nltools_assertAlways(msg.unmodulateables.size() == unMod);
  nltools_assertAlways(msg.macros.size() == mc);
  nltools_assertAlways(msg.hwamounts.size() == modR);
  return msg;
}

template <typename tMsg> void fillDualMessage(tMsg &msg, EditBuffer *editBuffer)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    size_t modP = 0;
    size_t unMod = 0;

    insertMockedParameters(msg, vg, unMod, modP);
    nltools_assertAlways(unMod == 7);
    nltools_assertAlways(modP == 8);

    auto arrayIndex = static_cast<int>(vg);
    for(auto &g : editBuffer->getParameterGroups(vg))
    {
      for(auto p : g->getParameters())
      {
        if(auto modParam = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &mod = msg.modulateables[arrayIndex][modP++];
          mod.id = modParam->getID().getNumber();
          mod.controlPosition = modParam->getControlPositionValue();
          mod.modulationAmount = modParam->getModulationAmount();
          mod.mc = modParam->getModulationSource();
        }
        else
        {
          if(p->getID().getNumber() == 249)
          {
            auto &unisonVoices = msg.unisonVoices[arrayIndex];
            unisonVoices.id = 249;
            unisonVoices.controlPosition = p->getControlPositionValue();
            unisonVoices.locked = p->isLocked();
          }
          else
          {
            auto &unModulateable = msg.unmodulateables[arrayIndex][unMod++];
            unModulateable.id = p->getID().getNumber();
            unModulateable.controlPosition = p->getControlPositionValue();
            unModulateable.locked = p->isLocked();
          }
        }
      }
    }

    nltools_assertAlways(msg.modulateables[arrayIndex].size() == modP);
    nltools_assertAlways(msg.unmodulateables[arrayIndex].size() == unMod);
  }
}

nltools::msg::SplitPresetMessage AudioEngineProxy::createSplitEditBufferMessage()
{
  nltools::msg::SplitPresetMessage msg{};
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  fillMessageWithGlobalParams(msg, editBuffer);

  fillDualMessage(msg, editBuffer);

  if(auto sp = editBuffer->getSplitPoint())
  {
    auto &t = msg.splitpoint;
    t.id = sp->getID().getNumber();
    t.controlPosition = sp->getControlPositionValue();
  }

  return msg;
}

nltools::msg::LayerPresetMessage AudioEngineProxy::createLayerEditBufferMessage()
{
  nltools::msg::LayerPresetMessage msg{};
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  fillMessageWithGlobalParams(msg, editBuffer);
  fillDualMessage(msg, editBuffer);
  return msg;
}

void AudioEngineProxy::sendEditBuffer()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  switch(eb->getType())
  {
    case SoundType::Single:
      nltools::msg::send(nltools::msg::EndPoint::AudioEngine, createSingleEditBufferMessage());
      break;
    case SoundType::Split:
      nltools::msg::send(nltools::msg::EndPoint::AudioEngine, createSplitEditBufferMessage());
      break;
    case SoundType::Layer:
      nltools::msg::send(nltools::msg::EndPoint::AudioEngine, createLayerEditBufferMessage());
      break;
    default:
      return;
  }
}
