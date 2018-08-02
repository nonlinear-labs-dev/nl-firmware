#pragma once

#include <proxies/hwui/controls/DotSlider.h>

class StaticKnubbelSlider : public DotSlider {
public:
    static const int c_handleWidth = 4;
    StaticKnubbelSlider(Parameter *parameter, Rect pos);
    void onParamValueChanged (const Parameter* param) override;
    void drawHandle (FrameBuffer &fb) override;
};
