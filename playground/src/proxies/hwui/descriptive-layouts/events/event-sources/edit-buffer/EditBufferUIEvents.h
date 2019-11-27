#pragma once

#include <parameters/SplitPointParameter.h>
#include "EditBufferEvent.h"

namespace DescriptiveLayouts
{
  class SoundEditHeading : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto type = eb->getType();
      auto typeStr = toString(type);
      if(type == SoundType::Single)
        setValue({ typeStr + " " + eb->getName(), 0 });
      else
      {
        auto sel = Application::get().getHWUI()->getCurrentVoiceGroup();
        setValue({ typeStr + (sel == VoiceGroup::I ? " I" : " II") + " " + eb->getVoiceGroupName(sel), 0 });
      }
    }
  };

  class MonoEnabledText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(eb->getType() == SoundType::Split)
      {
        auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
        if(auto param = eb->findParameterByID({ 364, vg }))
        {
          setValue({ param->getDisplayString(), 0 });
        }
      }
      else
      {
        if(auto param = eb->findParameterByID({ 364, VoiceGroup::I }))
          setValue({ param->getDisplayString(), 0 });
      }
    }
  };

  class MonoEnabledBool : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(eb->getType() == SoundType::Split)
      {
        auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
        if(auto param = eb->findParameterByID({ 364, vg }))
          setValue(param->getDisplayString() == "On");
      }
      else
      {
        if(auto param = eb->findParameterByID({ 364, VoiceGroup::I }))
          setValue(param->getDisplayString() == "On");
      }
    }
  };

  class UnisonEnabledBool : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(eb->getType() == SoundType::Split)
      {
        auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
        if(auto param = eb->findParameterByID({ 249, vg }))
          setValue(param->getControlPositionValue() > 0);
      }
      else
      {
        if(auto param = eb->findParameterByID({ 249, VoiceGroup::I }))
          setValue(param->getControlPositionValue() > 0);
      }
    }
  };

  class SelectVGButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto currentType = eb->getType();
      if(currentType != SoundType::Single)
      {
        auto sel = Application::get().getHWUI()->getCurrentVoiceGroup();
        setValue({ sel == VoiceGroup::I ? "Select II" : "Select I", 0 });
      }
      else
      {
        setValue({ "", 0 });
      }
    }
  };

  class VoicesParameterHeader : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto type = eb->getType();
      if(type == SoundType::Split)
      {
        auto selection = Application::get().getHWUI()->getCurrentVoiceGroup();
        setValue({ "Voices " + toString(selection), 0 });
      }
      else if(type == SoundType::Layer)
      {
        setValue({ "Voices I/II", 0 });
      }
      else
      {
        setValue({ "Voices", 0 });
      }
    }
  };

  template <int id, VoiceGroup vg> class ParameterValue : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(auto param = eb->findParameterByID({ id, vg }))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  template <int id> class ParameterValueCurrentVG : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
      if(auto param = eb->findParameterByID({ id, vg }))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  class UnisonVoicesText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *editBuffer)
    {
      if(editBuffer->getType() == SoundType::Split)
      {
        auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
        if(auto param = editBuffer->findParameterByID({ 249, vg }))
          setValue({ param->getControlPositionValue() == 0 ? "Off" : param->getDisplayString(), 0 });
      }
      else
      {
        if(auto param = editBuffer->findParameterByID({ 249, VoiceGroup::I }))
          setValue({ param->getControlPositionValue() == 0 ? "Off" : param->getDisplayString(), 0 });
      }
    }
  };

  class SplitPointValueText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
      if(auto splitPoint = eb->getSplitPoint())
      {
        setValue({ splitPoint->getDisplayValue(vg), 0 });
      }
      else
      {
        setValue({ "", 0 });
      }
    }
  };
}
