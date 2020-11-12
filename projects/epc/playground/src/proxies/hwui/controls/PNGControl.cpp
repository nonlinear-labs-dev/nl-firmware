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

void PNGControl::setOffset(const std::pair<int, int>& offset)
{
  offsetX = offset.first;
  offsetY = offset.second;
}

bool PNGControl::redraw(FrameBuffer& fb)
{
  using FBC = FrameBufferColors;

  auto pos = getPosition();

  if(!m_transparent && isVisible())
  {
    fb.setColor(FBC::C43);
    fb.fillRect(getPosition());
  }

  if(m_imagePath.empty())
  {
    return true;
  }

  for(auto y = 0; y < m_image.get_height(); y++)
  {
    for(auto x = 0; x < m_image.get_width(); x++)
    {
      auto pixel = m_image.get_pixel(x, y);
      if(pixel.alpha != 0)
      {
        fb.setColor(m_color);
        fb.setPixel(pos.getLeft() + x + offsetX, pos.getTop() + y + offsetY);
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
  if(l.empty())
  {
    m_imagePath = "";
  }

  try
  {
    m_image.read(getResourcesDir() + "/png/" + l);
    m_imagePath = getResourcesDir() + "/png/" + l;
  }
  catch(const std::runtime_error& err)
  {
    nltools::Log::error(__LINE__, __FILE__, err.what(), "File: ", l);
  }
}

void PNGControl::setColor(FrameBufferColors color)
{
  m_color = color;
  setDirty();
}

void PNGControl::setTransparent(bool transparent)
{
  m_transparent = transparent;
  setDirty();
}
