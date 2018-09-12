#pragma once

#include <tools/Throttler.h>
#include <io/Sender.h>
#include <stddef.h>
#include <linux/fb.h>

struct fb_var_screeninfo;

class FrameBufferSender : public Sender
{
    using super = Sender;

  public:
    FrameBufferSender();
    virtual ~FrameBufferSender();

    void send(tMessage msg) override;

  private:
    using tPixel = uint8_t;

    Throttler m_throttler;
    int m_fd = -1;
    size_t m_buffersize = 0;
    tPixel *m_frontBuffer = nullptr;
    struct fb_var_screeninfo m_varInfo;
};
