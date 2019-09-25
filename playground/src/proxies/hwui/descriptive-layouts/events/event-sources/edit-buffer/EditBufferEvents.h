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

      if(type == EditBufferType::Single)
      {
        setValue({ typeStr, 0 });
      }
      else
      {
        const auto suffix = " I";
        const auto suffixLen = strlen(suffix);
        const auto str = typeStr + suffix;
        setValue({ str, suffixLen });
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
      setValue({ eb->getCurrentVoiceGroupName(), 0 });
    }
  };

  class IsCurrentVGI : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      setValue(true);
    }
  };

  class IsCurrentVGII : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      setValue(false);
    }
  };
}
