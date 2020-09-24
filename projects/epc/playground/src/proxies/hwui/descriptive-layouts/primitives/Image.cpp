#include <any>
#include <nltools/logging/Log.h>
#include "Image.h"

DescriptiveLayouts::Image::Image(const DescriptiveLayouts::PrimitiveInstance& instance)
    : PNGControl(instance.relativePosition)
    , m_primitive { instance }
{
}

void DescriptiveLayouts::Image::setProperty(DescriptiveLayouts::PrimitiveProperty key, std::any value)
{
  if(key == PrimitiveProperty::ImagePath)
  {
    try
    {
      auto v = std::any_cast<std::string>(value);
      loadImage(v);
    }
    catch(...)
    {
      auto curr = std::current_exception();
      nltools::Log::error(nltools::handle_eptr(curr));
    }
  }
}

const DescriptiveLayouts::PrimitiveInstance& DescriptiveLayouts::Image::getPrimitive() const
{
  return m_primitive;
}

void DescriptiveLayouts::Image::setDirty()
{
  PNGControl::setDirty();
}

bool DescriptiveLayouts::Image::redraw(FrameBuffer& fb)
{
  setColor((FrameBufferColors) getStyleValue(StyleKey::Color));
  return PNGControl::redraw(fb);
}
