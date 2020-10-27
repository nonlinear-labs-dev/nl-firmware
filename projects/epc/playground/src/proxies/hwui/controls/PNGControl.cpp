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

  static std::vector<std::pair<int, FBC>> colors
      = { { 43, FBC::C43 },   { 77, FBC::C77 },   { 103, FBC::C103 }, { 128, FBC::C128 },
          { 179, FBC::C179 }, { 204, FBC::C204 }, { 255, FBC::C255 } };

  auto nearestColorForGrey = [&](int grey) {
    auto minDiff = std::numeric_limits<int>::max();
    auto nearestColor = FBC::C43;

    for(auto c : colors)
    {
      auto diff = std::abs(c.first - grey);
      if(diff < minDiff)
      {
        minDiff = diff;
        nearestColor = c.second;
      }
    }

    return nearestColor;
  };

  if(!isVisible())
  {
    return true;
  }

  for(auto y = 0; y < m_image.get_height(); y++)
  {
    for(auto x = 0; x < m_image.get_width(); x++)
    {
      auto pixel = m_image.get_pixel(x, y);
      if(m_useColors)
      {
        auto greyRaw = pixel.red;

        if(pixel.alpha != 0)
        {
          fb.setColor(nearestColorForGrey(greyRaw));
          fb.setPixel(pos.getLeft() + x + offsetX, pos.getTop() + y + offsetY);
        }
      }
      else
      {
        if(pixel.alpha != 0)
        {
          fb.setColor(m_color);
          fb.setPixel(pos.getLeft() + x + offsetX, pos.getTop() + y + offsetY);
        }
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
    nltools::Log::error(__LINE__, __FILE__, err.what());
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

void PNGControl::useImageColors(bool useColors)
{
  m_useColors = useColors;
  setDirty();
}
