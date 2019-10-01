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
        const auto suffix = " " + toString(Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection());
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

  class CurrentVoiceGroupName : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto name = eb->getName(Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection());
      setValue({ name, 0 });
    }
  };

  class IsCurrentVGI : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto val = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();
      setValue(val == VoiceGroup::I);
    }
  };

  class IsCurrentVGII : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto val = Application::get().getEditBufferSelectionForHardwareUI()->getEditBufferSelection();

      setValue(val == VoiceGroup::II);
    }
  };
}
