#include <png++/image.hpp>
#include <CompileTimeOptions.h>
#include "PNGControl.h"
#include <map>
#include <proxies/hwui/FrameBuffer.h>
#include <nltools/logging/Log.h>

PNGControl::PNGControl(const Rect& pos)
    : Control(pos)
{
}

PNGControl::PNGControl(const Rect& pos, const std::string& imagePath)
    : Control(pos)
{
  loadImage(imagePath);
}

int mapValue(unsigned char x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool PNGControl::redraw(FrameBuffer& fb)
{
  using FBC = FrameBufferColors;

  auto pos = getPosition();

  for(auto y = 0; y < m_image.get_width(); y++)
  {
    for(auto x = 0; x < m_image.get_height(); x++)
    {
      auto pixel = m_image.get_pixel(x, y);
      if(pixel.alpha != 0)
      {
        fb.setColor(m_color);
        fb.setPixel(pos.getLeft() + x, pos.getTop() + y);
      }
    }
  }
  return true;
}

void PNGControl::drawBackground(FrameBuffer& fb)
{
}

void PNGControl::loadImage(const std::string& l)
{
  try
  {
    m_image.read(l);
    m_imagePath = l;
  }
  catch(const std::runtime_error& err)
  {
    nltools::Log::error(__LINE__, __FILE__, err.what());
  }
}

void PNGControl::setColor(FrameBufferColors color)
{
  m_color = color;
  setDirty();
}
