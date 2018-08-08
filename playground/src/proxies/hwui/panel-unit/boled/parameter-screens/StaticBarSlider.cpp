#include "StaticBarSlider.h"


StaticBarSlider::StaticBarSlider(Parameter *param, const Rect &pos)
        : BarSlider(param, pos) {
    setHighlight(true);
    setValue(param->getControlPositionValue(), param->isBiPolar());
}

void StaticBarSlider::onParamValueChanged(const Parameter* param) {
}