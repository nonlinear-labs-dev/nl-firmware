#pragma once

#include <proxies/hwui/controls/Control.h>
#include <proxies/hwui/descriptive-layouts/Styleable.h>
#include <proxies/hwui/descriptive-layouts/PropertyOwner.h>
#include <proxies/hwui/descriptive-layouts/PrimitiveInstance.h>
#include <proxies/hwui/controls/PNGControl.h>

namespace DescriptiveLayouts
{
  class Image : public PNGControl, public Styleable, public PropertyOwner
  {
   public:
    explicit Image(const PrimitiveInstance& instance);
    void setProperty(PrimitiveProperty key, std::any value) override;
    const PrimitiveInstance& getPrimitive() const override;
    bool redraw(FrameBuffer& fb) override;

   protected:
    void setDirty() override;

   private:
    PrimitiveInstance m_primitive;
  };
}