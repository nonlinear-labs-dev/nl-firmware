#include <proxies/hwui/controls/Button.h>
#include "ParameterRecallLayout.h"


ParameterRecallLayout::ParameterRecallLayout(ParameterLayout2 *parent) : DFBLayout(getOLEDProxy()), m_parentLayout(parent) {
    addControl(new Button("Reset", BUTTON_A));
}

bool ParameterRecallLayout::onButton (int i, bool down, ButtonModifiers modifiers) {

}

bool ParameterRecallLayout::onRotary (int inc, ButtonModifiers modifiers) {

}