#pragma once

#include "../base/EventSource.h"
#include <nltools/Types.h>

class EditBuffer;

namespace DescriptiveLayouts
{
  class EditBufferEventBase
  {
   public:
    EditBufferEventBase();
    virtual ~EditBufferEventBase();

    virtual void onEditBufferChanged();
    virtual void onPresetLoaded();
    virtual void onRecallChanged();
    virtual void onHWUIVoiceGroupSelectionChanged(VoiceGroup v);
    virtual void onChange(const EditBuffer* eb) = 0;
    static EditBuffer* getEditBuffer();

   private:
    sigc::connection m_changedConnection;
    sigc::connection m_presetLoadedConnection;
    sigc::connection m_onRecallChanged;
    sigc::connection m_onHardwareUIVoiceGroupSelectionChanged;
  };

  template <typename T> class EditBufferEvent : public EventSource<T>, public EditBufferEventBase
  {
   public:
    EditBufferEvent()
    {
    }

    ~EditBufferEvent() override
    {
    }
  };
}
