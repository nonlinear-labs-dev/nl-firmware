#include <proxies/hwui/panel-unit/boled/setup/AboutText.h>

class Rect;

AboutText::AboutText () :
    super ("")
{
  setText("Development:\nStephan Schmitt, Daniel Tzschentke, Simone Fabbri, "
      "Henry Högelow, Matthias Seeber, Pascal Hürst, Justus Scheil, "
      "Klaus Strohhäcker, Tino Meinert, Anton Schmied, Gabriel Mulzer, "
      "Nemanja Nikodijevic, Mike Daliot, Ulrich Wappler",
      FrameBuffer::Colors::C255);
}

void AboutText::setPosition (const Rect &rect)
{
  super::setPosition (rect);
}

const Rect &AboutText::getPosition () const
{
  return super::getPosition ();
}

void AboutText::setDirty ()
{
  super::setDirty ();
  notifyDirty (true);
}
