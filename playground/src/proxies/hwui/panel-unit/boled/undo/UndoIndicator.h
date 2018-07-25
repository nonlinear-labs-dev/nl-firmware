#include <proxies/hwui/controls/Control.h>

class UndoIndicator : public Control {
public:
    UndoIndicator(const Rect &pos);
    bool redraw (FrameBuffer &fb) override;
};
