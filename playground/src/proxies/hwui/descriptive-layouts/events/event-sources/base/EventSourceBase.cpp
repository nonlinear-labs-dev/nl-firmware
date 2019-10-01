#include "EventSourceBase.h"
namespace DescriptiveLayouts
{

  sigc::connection EventSourceBase::connect(const EventSourceBase::Callback &cb)
  {
    return m_outputSignal.connectAndInit(cb, getLastValue());
  }
}
