#pragma once

#include <playground.h>
#include <experimental/any>
#include <sigc++/connection.h>
#include <tools/Signal.h>

namespace DescriptiveLayouts
{

  class EventSourceBase : public sigc::trackable
  {
   public:
    virtual ~EventSourceBase() = default;

    using Callback = std::function<void(std::experimental::any)>;
    sigc::connection connect(const Callback &cb);

   protected:
    virtual std::experimental::any getLastValue() const = 0;
    Signal<void, std::experimental::any> m_outputSignal;
    friend class EventSourceBroker;
  };
}
