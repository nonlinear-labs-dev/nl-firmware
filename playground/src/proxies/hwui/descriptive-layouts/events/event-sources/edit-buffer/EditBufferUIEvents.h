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
        setValue({ typeStr, 0 });
      else
      {
        auto sel = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
        setValue({ typeStr + (sel == VoiceGroup::I ? "[II]" : "[I]"), 0 });
      }
    }
  };

  class MonoEnabledText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      if(auto param = eb->findParameterByID(12345, vg))
      {
        setValue({ param->getDisplayString(), 0 });
      }
    }
  };

  class MonoEnabledBool : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      if(auto param = eb->findParameterByID(12345, vg))
        setValue(param->getDisplayString() == "On");
    }
  };

  class UnisonEnabledBool : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      if(auto param = eb->findParameterByID(249, vg))
        setValue(param->getControlPositionValue() > 0);
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
        auto sel = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
        setValue({ sel == VoiceGroup::I ? "Select II" : "Select I", 0 });
      }
      else
      {
        setValue({ "", 0 });
      }
    }
  };

  class EditBufferMasterVolumeText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto param = eb->findParameterByID(247);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class EditBufferMasterTuneText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto param = eb->findParameterByID(248);
      setValue({ param->getDisplayString(), 0 });
    }
  };

  class VoicesParameterHeader : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto selection = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      setValue({ "Voices " + toString(selection), 0 });
    }
  };

  template <int id, VoiceGroup vg> class ParameterValue : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(auto param = eb->findParameterByID(id, vg))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  template <int id> class ParameterValueCurrentVG : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      if(auto param = eb->findParameterByID(id, vg))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  class UnisonVoicesText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *editBuffer)
    {
      auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      if(auto param = editBuffer->findParameterByID(249, vg))
        setValue({ param->getControlPositionValue() == 0 ? "Off" : param->getDisplayString(), 0 });
    }
  };

  class VGIMasterVolumeText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(auto param = eb->findParameterByID(11247, VoiceGroup::I))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIIMasterVolumeText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(auto param = eb->findParameterByID(11247, VoiceGroup::II))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIMasterTuneText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(auto param = eb->findParameterByID(248, VoiceGroup::I))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  class VGIIMasterTuneText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      if(auto param = eb->findParameterByID(248, VoiceGroup::II))
        setValue({ param->getDisplayString(), 0 });
    }
  };

  class SplitPointValueText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override
    {
      auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      if(auto splitPoint = dynamic_cast<const SplitPointParameter *>(eb->getSplitPoint()))  //TODO change 18700
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
