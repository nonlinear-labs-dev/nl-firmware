#pragma once

#include "EditBufferEvent.h"

namespace DescriptiveLayouts
{
  class EditBufferTypeStringEvent : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      const auto type = eb->getType();
      const auto typeStr = toString(type);

      if(type == SoundType::Single)
      {
        setValue({ typeStr, 0 });
      }
      else
      {
        const auto suffix
            = " " + toString(Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection());
        setValue({ typeStr + suffix, suffix.size() });
      }
    }
  };

  class EditBufferName : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      setValue({ eb->getName(), 0 });
    }
  };

  class CurrentVoiceGroupLabel : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto currentVG = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      setValue({ eb->getVoiceGroupName(currentVG), 0 });
    }
  };

  class IsCurrentVGI : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto val = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      setValue(val == VoiceGroup::I);
    }
  };

  class IsCurrentVGII : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto val = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();

      setValue(val == VoiceGroup::II);
    }
  };

  class SoundParamsButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto base = "Params..";
      auto suffix = isChanged(eb) ? "*" : "";
      setValue({ std::string(base) + suffix, 0 });
    }

   private:
    bool isChanged(const EditBuffer *eb)
    {
      auto type = eb->getType();
      switch(type)
      {
        case SoundType::Invalid:
        case SoundType::Single:
          return false;
        case SoundType::Layer:
        case SoundType::Split:
        {
          auto vgI = eb->getParameterGroupByID("VGM", VoiceGroup::I)->isAnyParameterChanged();
          auto vgII = eb->getParameterGroupByID("VGM", VoiceGroup::II)->isAnyParameterChanged();
          if(type == SoundType::Split)
            return vgI || vgII || (eb->getSplitPoint() != nullptr && eb->getSplitPoint()->isChangedFromLoaded());
          return vgI || vgII;
        }
      }
      return false;
    }
  };

  class SoundVoicesButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto base = "Voices..";
      auto suffix = isChanged(eb) ? "*" : "";
      setValue({ std::string(base) + suffix, 0 });
    }

   private:
    bool isChanged(const EditBuffer *eb)
    {
      auto type = eb->getType();
      if(type == SoundType::Split)
      {
        auto monoI = eb->getParameterGroupByID("Mono", VoiceGroup::I);
        auto monoII = eb->getParameterGroupByID("Mono", VoiceGroup::II);
        auto vgUnisonI = eb->getParameterGroupByID("Unison", VoiceGroup::I);
        auto vgUnisonII = eb->getParameterGroupByID("Unison", VoiceGroup::II);
        return (monoI && monoI->isAnyParameterChanged()) || (monoII && monoII->isAnyParameterChanged())
            || (vgUnisonI && vgUnisonI->isAnyParameterChanged()) || (vgUnisonII && vgUnisonII->isAnyParameterChanged());
      }
      else if(type == SoundType::Layer || type == SoundType::Single)
      {
        auto mono = eb->getParameterGroupByID("Mono", VoiceGroup::I);
        auto vgUnison = eb->getParameterGroupByID("Unison", VoiceGroup::I);
        return (mono && mono->isAnyParameterChanged()) || (vgUnison && vgUnison->isAnyParameterChanged());
      }
      return false;
    }
  };

  class SoundMasterButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto base = "Master..";
      auto suffix = isChanged(eb) ? "*" : "";
      setValue({ std::string(base) + suffix, 0 });
    }

   private:
    bool isChanged(const EditBuffer *eb)
    {
      auto masterGroup = eb->getGlobalParameterGroupByID("Master");
      auto scale = eb->getGlobalParameterGroupByID("Scale");
      return (scale && scale->isAnyParameterChanged()) || (masterGroup && masterGroup->isAnyParameterChanged());
    }
  };

  class MonoButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto base = "Mono..";
      auto suffix = isChanged(eb) ? "*" : "";
      setValue({ std::string(base) + suffix, 0 });
    }

   private:
    bool isChanged(const EditBuffer *eb)
    {
      switch(eb->getType())
      {
        case SoundType::Single:
        case SoundType::Invalid:
        case SoundType::Layer:
        {
          auto mono = eb->getParameterGroupByID("Mono", VoiceGroup::I);
          return mono && mono->isAnyParameterChanged();
        }
        case SoundType::Split:
        {
          auto mono = eb->getParameterGroupByID("Mono");
          return mono && mono->isAnyParameterChanged();
        }
      }
      return false;
    }
  };

  class UnisonButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb)
    {
      auto base = "Unison..";
      auto suffix = isChanged(eb) ? "*" : "";
      setValue({ std::string(base) + suffix, 0 });
    }

   private:
    bool isChanged(const EditBuffer *eb)
    {
      switch(eb->getType())
      {
        case SoundType::Single:
        case SoundType::Invalid:
        case SoundType::Layer:
        {
          auto unison = eb->getParameterGroupByID("Unison", VoiceGroup::I);
          return unison && unison->isAnyParameterChanged();
        }
        case SoundType::Split:
        {
          auto unison = eb->getParameterGroupByID("Unison");
          return unison && unison->isAnyParameterChanged();
        }
      }
      return false;
    }
  };
}
