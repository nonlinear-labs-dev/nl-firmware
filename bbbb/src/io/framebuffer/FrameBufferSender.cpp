#include <Application.h>
#include <bbbb.h>
#include <fcntl.h>
#include <io/framebuffer/FrameBufferSender.h>
#include <Options.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>

FrameBufferSender::FrameBufferSender()
{
  m_fd = open(Application::get().getOptions()->getFrameBufferFileName(), O_RDWR);

  if(m_fd < 0)
  {
    TRACE("Could not open framebuffer device file");
    return;
  }

  if(ioctl(m_fd, FBIOGET_VSCREENINFO, &m_varInfo) == -1)
  {
    TRACE("Could not get fb_var_screeninfo!");
    return;
  }

  auto bytesPerPixel = m_varInfo.bits_per_pixel / 8;
  m_buffersize = m_varInfo.xres * m_varInfo.yres * bytesPerPixel;
  m_frontBuffer = reinterpret_cast<tPixel*>(mmap(0, m_buffersize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));

  if(m_frontBuffer == MAP_FAILED)
  {
    TRACE("Could not memory map buffer");
    return;
  }
}

FrameBufferSender::~FrameBufferSender()
{
}

void FrameBufferSender::send(tMessage msg)
{
  gsize numBytes = 0;
  auto bytes = msg->get_data(numBytes);
  constexpr auto messageHeaderLength = 6;

  if(numBytes >= messageHeaderLength)
  {
    struct __attribute__((packed)) FBMessage
    {
        uint16_t length;
        uint32_t idx;
        int8_t data[0];
    };

    const FBMessage *parsedMessage = reinterpret_cast<const FBMessage*>(bytes);

    if(parsedMessage->length == numBytes)
    {
      if(parsedMessage->idx + parsedMessage->length >= m_buffersize)
        TRACE("frame buffer message corrupted");
      else
        memcpy (m_frontBuffer + parsedMessage->idx, parsedMessage->data, parsedMessage->length - messageHeaderLength);
    }
  }
}
