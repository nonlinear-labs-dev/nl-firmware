#pragma once
#include <proxies/hwui/HWUIEnums.h>

class ButtonReceiver {
public:
    virtual bool onButton(int i, bool down, ButtonModifiers mod) = 0;
};
