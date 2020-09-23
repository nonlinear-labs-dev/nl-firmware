#include <png++/image.hpp>
#include <CompileTimeOptions.h>
#include "PNGControl.h"
#include <map>
#include <proxies/hwui/FrameBuffer.h>

PNGControl::PNGControl(const Rect& pos, const std::string& imagePath)
    : Control(pos)
    , m_image { getResourcesDir() + "/png/" + imagePath }
{
}

int mapValue(unsigned char x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool PNGControl::redraw(FrameBuffer& fb)
{
  using FBC = FrameBufferColors;
  static std::map<unsigned char, FrameBufferColors> pixelToColor
      = { { 0, FBC::Transparent }, { 1, FBC::C43 },  { 2, FBC::C77 },  { 3, FBC::C103 },
          { 4, FBC::C128 },        { 5, FBC::C179 }, { 6, FBC::C204 }, { 7, FBC::C255 } };

  auto pos = getPosition();

  for(auto y = 0; y < m_image.get_width(); y++)
  {
    for(auto x = 0; x < m_image.get_height(); x++)
    {
      auto pixel = m_image.get_pixel(x, y);
      if(pixel.alpha != 0)
      {
        //TODO communicate what colors will be used in png
        auto maxColor = std::max(pixel.blue, std::max(pixel.green, pixel.red));
        auto mapped = mapValue(maxColor, 0, 255, 0, 7);
        auto color = pixelToColor.at(mapped);
        if(color != FBC::Transparent)
        {
          fb.setColor(color);
          fb.setPixel(pos.getLeft() + x, pos.getTop() + y);
        }
      }
    }
  }
  return true;
}

void PNGControl::drawBackground(FrameBuffer& fb)
{
}
