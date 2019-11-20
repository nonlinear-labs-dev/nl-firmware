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

nltools::msg::SinglePresetMessage AudioEngineProxy::createSingleEditBufferMessage()
{
  nltools::msg::SinglePresetMessage msg;

  size_t aftertouch = 0;
  size_t bender = 0;
  size_t macros = 0;
  size_t modulateables = 0;
  size_t pedals = 0;
  size_t ribbons = 0;
  size_t unmodulateables = 0;
  size_t monos = 0;
  size_t scale = 0;
  size_t master = 0;
  size_t vgMaster = 0;

  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  for(auto vg : { VoiceGroup::Global, VoiceGroup::I })
  {
    for(auto &g : editBuffer->getParameterGroups(vg))
    {
      for(auto &p : g->getParameters())
      {
        if(auto a = dynamic_cast<AftertouchParameter *>(p))
        {
          auto &t = msg.aftertouch[aftertouch++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
          t.returnMode = a->getReturnMode();
        }
        else if(auto a = dynamic_cast<PitchbendParameter *>(p))
        {
          auto &t = msg.bender[bender++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
          t.returnMode = a->getReturnMode();
        }
        else if(auto a = dynamic_cast<MacroControlParameter *>(p))
        {
          auto &t = msg.macros[macros++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
        }
        else if(auto a = dynamic_cast<ModulateableParameter *>(p))
        {
          auto &t = msg.modulateables[modulateables++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
          t.mc = a->getModulationSource();
          t.modulationAmount = static_cast<double>(a->getModulationAmount());
        }
        else if(auto a = dynamic_cast<PedalParameter *>(p))
        {
          auto &t = msg.pedals[pedals++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
          t.pedalMode = a->getPedalMode();
          t.returnMode = a->getReturnMode();
        }
        else if(auto a = dynamic_cast<RibbonParameter *>(p))
        {
          auto &t = msg.ribbons[ribbons++];
          t.id = a->getID();
          t.controlPosition = static_cast<double>(a->getControlPositionValue());
          t.ribbonReturnMode = a->getRibbonReturnMode();
          t.ribbonTouchBehaviour = a->getRibbonTouchBehaviour();
        }
        else if(auto a = dynamic_cast<MonoParameter *>(p))
        {
          auto &t = msg.monos[monos++];
          t.id = p->getID();
          t.controlPosition = p->getControlPositionValue();
        }
        else if(auto a = dynamic_cast<VoiceGroupMasterParameter *>(p))
        {
          //IGNORE
          vgMaster++;
        }
        else if(auto a = dynamic_cast<ScaleParameter *>(p))
        {
          auto &t = msg.scale[scale++];
          t.id = a->getID();
          t.controlPosition = a->getControlPositionValue();
        }
        else if(auto a = dynamic_cast<Parameter *>(p))
        {
          if(a->getID() == 249)
          {
            auto &t = msg.unisonVoices;
            t.locked = a->isLocked();
            t.id = a->getID();
            t.controlPosition = t.controlPosition;
          }
          else if(a->getID() == 247)
          {
            auto &t = msg.master[master++];
            t.id = a->getID();
            t.controlPosition = a->getControlPositionValue();
          }
          else
          {
            auto &t = msg.unmodulateables[unmodulateables++];
            t.id = a->getID();
            t.controlPosition = static_cast<double>(a->getControlPositionValue());
          }
        }
      }
    }
  }

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
          auto &t = msg.monos[index][monos++];
          t.id = p->getID();
          t.controlPosition = p->getControlPositionValue();
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
            auto &t = msg.monos[monos++];
            t.id = p->getID();
            t.controlPosition = p->getControlPositionValue();
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
