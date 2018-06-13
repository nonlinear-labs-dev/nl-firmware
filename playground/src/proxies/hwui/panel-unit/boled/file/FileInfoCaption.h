#pragma once

#include <proxies/hwui/controls/Label.h>

class FileInfoCaption : public Label {
private:
    typedef Label super;
public:
    FileInfoCaption (const Glib::ustring& str, const Rect &pos);
    bool redraw (FrameBuffer &fb) override;
};