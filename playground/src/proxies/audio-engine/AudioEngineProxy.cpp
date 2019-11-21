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
        pItem.id = p->getID();
        pItem.controlPosition = p->getControlPositionValue();
        pItem.locked = p->isLocked();

        if(auto physicalControl = dynamic_cast<PhysicalControlParameter *>(p))
        {
          pItem.returnMode = physicalControl->getReturnMode();
        }
      }
      else if(isMaster || isScale)
      {
        auto &pItem = msg.globalparams[globalParams++];
        pItem.id = p->getID();
        pItem.controlPosition = p->getControlPositionValue();
      }
    }
  }

  nltools_assertAlways(msg.globalparams.size() == globalParams);
  nltools_assertAlways(msg.hwsources.size() == hwSource);
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

template <typename tMsg> bool handleMockModulateable(Parameter *p, tMsg &msg, size_t &modP)
{
  auto id = p->getID();
  if(id == 358 || id == 360 || id == 362 || id == 367)
  {
    auto &item = msg.modulateables[modP++];
    item.id = id;
    item.controlPosition = p->getControlPositionValue();
    item.modulationAmount = 0.0;
    item.mc = MacroControls::NONE;
    return true;
  }
  return false;
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

  for(auto &g : editBuffer->getParameterGroups(VoiceGroup::I))
  {
    for(auto p : g->getParameters())
    {
#warning "Mock"
      /*
      if(handleMockModulateable(p, msg, modP))
      {
        continue;
      }
       */

      if(auto mcParameter = dynamic_cast<MacroControlParameter *>(p))
      {
        auto &macro = msg.macros[mc++];
        macro.id = p->getID();
        macro.controlPosition = p->getControlPositionValue();
        macro.locked = p->isLocked();
      }
      else if(auto hwAmounts = dynamic_cast<ModulationRoutingParameter *>(p))
      {
        auto &hwAmount = msg.hwamounts[modR++];
        hwAmount.id = p->getID();
        hwAmount.controlPosition = p->getControlPositionValue();
        hwAmount.locked = p->isLocked();
      }
      else if(auto modParam = dynamic_cast<ModulateableParameter *>(p))
      {
        auto &mod = msg.modulateables[modP++];
        mod.id = p->getID();
        mod.controlPosition = modParam->getControlPositionValue();
        mod.modulationAmount = modParam->getModulationAmount();
        mod.mc = modParam->getModulationSource();
      }
      else
      {
        auto &unModulateable = msg.unmodulateables[unMod++];
        unModulateable.id = p->getID();
        unModulateable.controlPosition = p->getControlPositionValue();
        unModulateable.locked = p->isLocked();
      }
    }
  }

  nltools_assertAlways(msg.modulateables.size() == modP + 1);
  nltools_assertAlways(msg.unmodulateables.size() == unMod + 1);
  nltools_assertAlways(msg.macros.size() == mc + 1);
  nltools_assertAlways(msg.hwamounts.size() == modR + 1);
  return msg;
}

nltools::msg::SplitPresetMessage AudioEngineProxy::createSplitEditBufferMessage()
{
  nltools::msg::SplitPresetMessage msg;
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    const auto index = static_cast<int>(vg);

    size_t macros = 0;
    size_t modulateables = 0;
    size_t unmodulateables = 0;
    size_t monos = 0;
    size_t vgmaster = 0;

    for(auto &g : editBuffer->getParameterGroups(vg))
    {
      for(auto &p : g->getParameters())
      {
        if(auto a = dynamic_cast<MacroControlParameter *>(p))
        {
          auto &t = msg.macros[index][macros++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
        }
        else if(auto a = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &t = msg.modulateables[index][modulateables++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
          t.mc = a->getModulationSource();
          t.modulationAmount = static_cast<double>(a->getModulationAmount());
        }
        else if(auto a = dynamic_cast<MonoParameter *>(p))
        {
        }
        else if(auto a = dynamic_cast<VoiceGroupMasterParameter *>(p))
        {
          auto &t = msg.vgMaster[index][vgmaster++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
        }
        else if(auto a = dynamic_cast<Parameter *>(p))
        {
          if(a->getID() == 249)
          {
            auto &t = msg.unisonVoices.at(index);
            t.locked = a->isLocked();
            t.id = a->getID();
            t.controlPosition = t.controlPosition;
          }
          else
          {
            auto &t = msg.unmodulateables[index][unmodulateables++];
            t.id = a->getID();
            t.controlPosition = static_cast<double>(a->getControlPositionValue());
            t.locked = a->isLocked();
          }
        }
      }
    }
  }

  size_t master = 0;
  if(auto g = editBuffer->getParameterGroupByID("Master", VoiceGroup::Global))
  {
    for(auto &p : g->getParameters())
    {
      auto &t = msg.master[master++];
      t.id = p->getID();
      t.controlPosition = p->getControlPositionValue();
      t.locked = p->isLocked();
    }
  }

  size_t scale = 0;
  if(auto g = editBuffer->getParameterGroupByID("Scale", VoiceGroup::Global))
  {
    for(auto &p : g->getParameters())
    {
      auto &t = msg.scale[scale++];
      t.id = p->getID();
      t.controlPosition = p->getControlPositionValue();
      t.locked = p->isLocked();
    }
  }

  size_t pedal = 0;
  size_t ribbons = 0;
  size_t aftertouch = 0;
  size_t bender = 0;
  if(auto g = editBuffer->getParameterGroupByID("CS", VoiceGroup::Global))
  {
    for(auto &p : g->getParameters())
    {
      if(auto pedalParam = dynamic_cast<const PedalParameter *>(p))
      {
        auto &t = msg.pedals.at(pedal++);
        t.controlPosition = pedalParam->getControlPositionValue();
        t.id = pedalParam->getID();
        t.returnMode = pedalParam->getReturnMode();
        t.pedalMode = pedalParam->getPedalMode();
        t.locked = pedalParam->isLocked();
      }
      else if(auto ribbonParam = dynamic_cast<const RibbonParameter *>(p))
      {
        auto &t = msg.ribbons.at(ribbons++);
        t.id = ribbonParam->getID();
        t.controlPosition = ribbonParam->getControlPositionValue();
        t.locked = ribbonParam->isLocked();
        t.ribbonReturnMode = ribbonParam->getRibbonReturnMode();
        t.ribbonTouchBehaviour = ribbonParam->getRibbonTouchBehaviour();
      }
      else if(auto afterTouchParameter = dynamic_cast<const AftertouchParameter *>(p))
      {
        auto &t = msg.aftertouch.at(aftertouch++);
        t.id = afterTouchParameter->getID();
        t.controlPosition = afterTouchParameter->getControlPositionValue();
        t.locked = afterTouchParameter->isLocked();
        t.returnMode = afterTouchParameter->getReturnMode();
      }
      else if(auto pitchBendParameter = dynamic_cast<PitchbendParameter *>(p))
      {
        auto &t = msg.bender.at(bender++);
        t.id = pitchBendParameter->getID();
        t.controlPosition = pitchBendParameter->getControlPositionValue();
        t.returnMode = pitchBendParameter->getReturnMode();
      }
    }
  }

  if(auto sp = editBuffer->getSplitPoint())
  {
    auto &t = msg.splitpoint;
    t.id = sp->getID();
    t.controlPosition = sp->getControlPositionValue();
  }

  return msg;
}

nltools::msg::LayerPresetMessage AudioEngineProxy::createLayerEditBufferMessage()
{
  nltools::msg::LayerPresetMessage msg;
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    const auto index = static_cast<int>(vg);

    size_t macros = 0;
    size_t modulateables = 0;
    size_t unmodulateables = 0;
    size_t monos = 0;
    size_t vgmaster = 0;

    for(auto &g : editBuffer->getParameterGroups(vg))
    {
      for(auto &p : g->getParameters())
      {
        if(auto a = dynamic_cast<MacroControlParameter *>(p))
        {
          auto &t = msg.macros[index][macros++];
          t.id = a->getID();
          t.controlPosition = a->getControlPositionValue();
        }
        else if(auto a = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &t = msg.modulateables[index][modulateables++];
          t.id = a->getID();
          t.controlPosition = a->getControlPositionValue();
          t.mc = a->getModulationSource();
          t.modulationAmount = a->getModulationAmount();
        }
        else if(auto a = dynamic_cast<MonoParameter *>(p))
        {
          if(index == 0)
          {
          }
        }
        else if(auto a = dynamic_cast<VoiceGroupMasterParameter *>(p))
        {
          auto &t = msg.vgMaster[index][vgmaster++];
          t.id = a->getID();
          t.controlPosition = a->getControlPositionValue();
        }
        else if(auto a = dynamic_cast<Parameter *>(p))
        {
          if(a->getID() == 249)
          {
            auto &t = msg.unisonVoices.at(index);
            t.locked = a->isLocked();
            t.id = a->getID();
            t.controlPosition = t.controlPosition;
          }
          else
          {
            auto &t = msg.unmodulateables[index][unmodulateables++];
            t.id = a->getID();
            t.controlPosition = static_cast<double>(a->getControlPositionValue());
            t.locked = a->isLocked();
          }
        }
      }
    }
  }

  size_t master = 0;
  if(auto g = editBuffer->getParameterGroupByID("Master", VoiceGroup::Global))
  {
    for(auto &p : g->getParameters())
    {
      auto &t = msg.master[master++];
      t.id = p->getID();
      t.controlPosition = p->getControlPositionValue();
    }
  }

  size_t scale = 0;
  if(auto g = editBuffer->getParameterGroupByID("Scale", VoiceGroup::Global))
  {
    for(auto &p : g->getParameters())
    {
      auto &t = msg.scale[scale++];
      t.id = p->getID();
      t.controlPosition = p->getControlPositionValue();
    }
  }

  size_t aftertouch = 0;
  size_t pedal = 0;
  size_t bender = 0;
  size_t ribbons = 0;

  if(auto g = editBuffer->getParameterGroupByID("CS", VoiceGroup::Global))
  {
    for(auto &p : g->getParameters())
    {
      if(auto pedalParam = dynamic_cast<const PedalParameter *>(p))
      {
        auto &t = msg.pedals.at(pedal++);
        t.controlPosition = pedalParam->getControlPositionValue();
        t.id = pedalParam->getID();
        t.returnMode = pedalParam->getReturnMode();
        t.pedalMode = pedalParam->getPedalMode();
        t.locked = pedalParam->isLocked();
      }
      else if(auto ribbonParam = dynamic_cast<const RibbonParameter *>(p))
      {
        auto &t = msg.ribbons.at(ribbons++);
        t.id = ribbonParam->getID();
        t.controlPosition = ribbonParam->getControlPositionValue();
        t.locked = ribbonParam->isLocked();
        t.ribbonReturnMode = ribbonParam->getRibbonReturnMode();
        t.ribbonTouchBehaviour = ribbonParam->getRibbonTouchBehaviour();
      }
      else if(auto afterTouchParameter = dynamic_cast<const AftertouchParameter *>(p))
      {
        auto &t = msg.aftertouch.at(aftertouch++);
        t.id = afterTouchParameter->getID();
        t.controlPosition = afterTouchParameter->getControlPositionValue();
        t.locked = afterTouchParameter->isLocked();
        t.returnMode = afterTouchParameter->getReturnMode();
      }
      else if(auto pitchBendParameter = dynamic_cast<PitchbendParameter *>(p))
      {
        auto &t = msg.bender.at(bender++);
        t.id = pitchBendParameter->getID();
        t.controlPosition = pitchBendParameter->getControlPositionValue();
        t.returnMode = pitchBendParameter->getReturnMode();
      }
    }
  }

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
