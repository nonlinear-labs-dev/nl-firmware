#include <proxies/hwui/panel-unit/boled/setup/AboutText.h>

class Rect;

AboutText::AboutText()
    : super("")
{
  setText("Development and Design:\n"
          "Stephan Schmitt, Henry Högelow, Matthias Seeber, "
          "Jan Justus Scheil, Klaus Strohhäcker, Anton Schmied, "
          "Tino Meinert, Daniel Tzschentke, Simone Fabbri, "
          "Christian Radtke, Pascal Hürst, Nemanja Nikodijevic, "
          "Gabriel Mulzer, Mike Daliot, Paul Schulz, "
          "Thomas Jaufmann, David Johannes Meyer, Ulrich Wappler",
          FrameBufferColors::C255);
}

void AboutText::setPosition(const Rect &rect)
{
  super::setPosition(rect);
}

const Rect &AboutText::getPosition() const
{
  return super::getPosition();
}

void AboutText::setDirty()
{
  super::setDirty();
  notifyDirty(true);
}
