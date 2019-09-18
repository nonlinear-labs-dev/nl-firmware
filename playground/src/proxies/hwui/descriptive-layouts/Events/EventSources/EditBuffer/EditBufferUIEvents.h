#pragma once
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
      if(type == Type::Single)
        setValue({ typeStr, 0 });
      else
        setValue({ typeStr + (eb->isVGISelected() ? " [I]" : " [II]"), 0 });
    }
  };

  class SelectVGButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      setValue({ eb->isVGISelected() ? "Select II" : "Select I", 0 });
    }
  };

  class EditBufferMasterText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto param = eb->findParameterByID(247);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class EditBufferUnisonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto param = eb->findParameterByID(250);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIMasterText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      //TODO add voice group id!
      auto param = eb->findParameterByID(247);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIIMasterText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      //TODO add voice group id!
      auto param = eb->findParameterByID(247);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIUnisonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      //TODO add voice group id!
      auto param = eb->findParameterByID(250);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIIUnisonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      //TODO add voice group id!
      auto param = eb->findParameterByID(250);
      setValue({ param->getDisplayString(), 0 });
    }
  };
}