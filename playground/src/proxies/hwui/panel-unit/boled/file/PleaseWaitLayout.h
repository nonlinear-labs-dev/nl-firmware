
#include <proxies/hwui/DFBLayout.h>

class PleaseWaitLayout : public DFBLayout {
public:
    PleaseWaitLayout();
    bool onButton(int i, bool down, ButtonModifiers modifiers) override;
};

