#include "StaticKnubbelSlider.h"

StaticKnubbelSlider::StaticKnubbelSlider(Parameter *parameter, Rect pos)
        : DotSlider(parameter, pos) {

}

void StaticKnubbelSlider::onParamValueChanged (const Parameter* param)
{
}

void StaticKnubbelSlider::drawHandle(FrameBuffer &fb) {
    const Rect &r = getPosition ();
    const int handleHeight = r.getHeight();
    auto halfHandle = c_handleWidth / 2;

    int x = getHandlePixelPosition ();

    if (!isHighlight())
    {
        fb.setColor (FrameBuffer::Colors::C103);
        fb.fillRect (x - halfHandle, r.getTop (), c_handleWidth, handleHeight);
        fb.setColor (FrameBuffer::Colors::C179);
    }
    else
    {
        fb.setColor (FrameBuffer::Colors::C255);
    }

    fb.fillRect (x + 1 - halfHandle, r.getTop (), c_handleWidth - 2, handleHeight);
    fb.fillRect (x - halfHandle, r.getTop () + 1, c_handleWidth, handleHeight - 2);
}
