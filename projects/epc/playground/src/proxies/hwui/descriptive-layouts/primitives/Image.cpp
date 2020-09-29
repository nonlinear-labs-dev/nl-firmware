#include <experimental/any>
#include <nltools/logging/Log.h>
#include "Image.h"

DescriptiveLayouts::Image::Image(const DescriptiveLayouts::PrimitiveInstance& instance)
    : PNGControl(instance.relativePosition)
    , m_primitive { instance }
{
}

void DescriptiveLayouts::Image::setProperty(DescriptiveLayouts::PrimitiveProperty key, std::experimental::any value)
{
  try
  {
    if(key == PrimitiveProperty::Visibility)
      setVisible(std::experimental::any_cast<bool>(value));
    if(key == PrimitiveProperty::ImagePath)
      loadImage(std::experimental::any_cast<std::string>(value));
    if(key == PrimitiveProperty::ImageOffset)
      setOffset(std::experimental::any_cast<std::pair<int, int>>(value));
    if(key == PrimitiveProperty::ImageTransparency)
      setTransparent(std::experimental::any_cast<bool>(value));
  }
  catch(...)
  {
    auto curr = std::current_exception();
    nltools::Log::error(nltools::handle_eptr(curr));
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

void DescriptiveLayouts::Image::drawBackground(FrameBuffer& fb)
{
  Control::drawBackground(fb);
}

bool DescriptiveLayouts::Image::redraw(FrameBuffer& fb)
{
  setColor((FrameBufferColors) getStyleValue(StyleKey::Color));
  return PNGControl::redraw(fb);
}
