#include <png++/image.hpp>
#include <CompileTimeOptions.h>
#include "PNGControl.h"
#include <map>
#include <proxies/hwui/FrameBuffer.h>
#include <nltools/logging/Log.h>

PNGControl::PNGControl(const Rect& pos, const std::string& imagePath)
    : Control(pos)
    , m_image { getResourcesDir() + "/png/" + imagePath }
    , m_imagePath { imagePath }
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
      = { { 0, FBC::C43 },  { 1, FBC::C77 },  { 2, FBC::C103 }, { 3, FBC::C128 },
          { 4, FBC::C179 }, { 5, FBC::C204 }, { 6, FBC::C255 } };

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
        auto mapped = mapValue(maxColor, 0, 255, 0, 6);

        try
        {
          auto color = pixelToColor.at(mapped);
          fb.setColor(color);
          fb.setPixel(pos.getLeft() + x, pos.getTop() + y);
        }
        catch(...)
        {
          nltools::Log::error("Invalid color at:", x, "/", y, "in file", m_imagePath);
        }
      }
    }
  }
  return true;
}

void PNGControl::drawBackground(FrameBuffer& fb)
{
}
