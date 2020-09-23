#pragma once
#include "Control.h"

class PNGControl : public Control
{
 public:
  PNGControl(const Rect& pos, const std::string& imagePath);
  bool redraw(FrameBuffer& fb) override;
  void drawBackground(FrameBuffer& fb) override;

 private:
  std::string m_imagePath;
  png::image<png::rgba_pixel> m_image;
};
