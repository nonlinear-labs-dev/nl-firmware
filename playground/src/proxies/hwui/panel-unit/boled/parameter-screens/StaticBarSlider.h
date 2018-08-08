#pragma once

#include <proxies/hwui/controls/BarSlider.h>

class StaticBarSlider : public BarSlider {
public:
    StaticBarSlider(Parameter *param, const Rect &pos);
    void onParamValueChanged(const Parameter* param) override;
};
