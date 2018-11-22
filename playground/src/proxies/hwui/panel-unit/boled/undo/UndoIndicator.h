#include <proxies/hwui/controls/Label.h>

class UndoIndicator : public Label
{
 public:
  UndoIndicator(const Rect &pos);
  bool redraw(FrameBuffer &fb) override;
};
