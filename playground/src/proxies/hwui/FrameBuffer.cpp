#include <device-settings/DebugLevel.h>
#include <fcntl.h>
#include <proxies/hwui/controls/Point.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <iostream>


#if _DEVELOPMENT_PC
size_t debugOledHeight = 64;
#else
size_t debugOledHeight = 64;
#endif

FrameBuffer::StackScopeGuard::StackScopeGuard (FrameBuffer *fb) :
    m_fb (fb)
{
}

FrameBuffer::StackScopeGuard::StackScopeGuard (StackScopeGuard &&other) :
    m_fb (nullptr)
{
  swap (m_fb, other.m_fb);
}

FrameBuffer::StackScopeGuard::~StackScopeGuard ()
{
}

FrameBuffer::Clip::Clip (FrameBuffer *fb, const Rect &rect) :
    StackScopeGuard (fb)
{
  auto intersection = rect.getIntersection (m_fb->m_clips.top ());
  fb->m_clips.push (intersection);
}

FrameBuffer::Clip::Clip (Clip &&other) :
    StackScopeGuard (std::move (other))
{
}

FrameBuffer::Clip::~Clip ()
{
  if (m_fb)
    m_fb->m_clips.pop ();
}

bool FrameBuffer::Clip::isEmpty () const
{
  return m_fb->m_clips.top ().isEmpty ();
}

FrameBuffer::Offset::Offset (FrameBuffer *fb, const Point &offset) :
    StackScopeGuard (fb)
{
  m_fb->m_offsets.push (m_fb->m_offsets.top () + offset);
}

FrameBuffer::Offset::Offset (Offset &&other) :
    StackScopeGuard (std::move (other))
{
}

FrameBuffer::Offset::~Offset ()
{
  if (m_fb)
    m_fb->m_offsets.pop ();
}

FrameBuffer::FrameBuffer ()
{
  initStacks ();
  openAndMap ();
  clear ();
}

void FrameBuffer::initStacks ()
{
  m_clips.push (Rect (0, 0, 256, 64 + 32 + debugOledHeight));
  m_offsets.push (Point (0, 0));
}

void FrameBuffer::openAndMap ()
{
  m_fd = open ("/dev/fb0", O_RDWR);

  if (m_fd < 0)
    g_error("Could not open framebuffer device file");

  if (ioctl (m_fd, FBIOGET_VSCREENINFO, &m_varInfo) == -1)
    g_error("Could not get fb_var_screeninfo!");

  auto bytesPerPixel = m_varInfo.bits_per_pixel / 8;
  m_buffersize = m_varInfo.xres * m_varInfo.yres * bytesPerPixel;
  m_frontBuffer = reinterpret_cast<tPixel*> (mmap (0, m_buffersize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));

  if (m_frontBuffer == MAP_FAILED)
    g_error("Could not memory map buffer");

  m_backBuffer.resize (m_varInfo.xres * m_varInfo.yres);
}

FrameBuffer::~FrameBuffer ()
{
  unmapAndClose ();
}

void FrameBuffer::setPixel (tCoordinate x, tCoordinate y)
{
  const auto& offset = m_offsets.top ();
  setOffsetPixel (x + offset.getX (), y + offset.getY ());
}

inline void FrameBuffer::setOffsetPixel (tCoordinate x, tCoordinate y)
{
  if (m_clips.top ().contains (x, y))
    setRawPixel (x, y);
}

inline void FrameBuffer::setRawPixel (tCoordinate x, tCoordinate y)
{
  const long index = getIndex (x, y);

#if _DEVELOPMENT_PC
  g_assert(x < 256);
  g_assert(y < (64 + 32 + debugOledHeight));
  g_assert((index * sizeof(tPixel)) < m_buffersize);
  m_frontBuffer[index] = m_currentColor;
#else
  m_backBuffer[index] = m_currentColor;
#endif
}

inline long FrameBuffer::getIndex (tCoordinate x, tCoordinate y) const
{
  return (x + y * m_varInfo.xres);
}

void FrameBuffer::unmapAndClose ()
{
  if (munmap (m_frontBuffer, m_buffersize))
    DebugLevel::error ("Error while un-mapping buffer: ", errno);

  if (close (m_fd))
    DebugLevel::error ("Error while closing file handle: ", errno);
}

void FrameBuffer::clear ()
{
#if _DEVELOPMENT_PC
  memset (m_frontBuffer, 0, m_buffersize);
#else
  memset (m_backBuffer.data (), 0, m_buffersize);
#endif
}

void FrameBuffer::setColor (const Colors &c)
{
  m_currentColor = c;
}

void FrameBuffer::fiddleColor (tPixel p)
{
  m_currentColor = (Colors) (p);
}

FrameBuffer::Colors FrameBuffer::getColor () const
{
  return m_currentColor;
}

void FrameBuffer::fillRect (tCoordinate left, tCoordinate top, tCoordinate width, tCoordinate height)
{
  fillRect (Rect (left, top, width, height));
}

void FrameBuffer::fillRect (const Rect &rect)
{
  Rect fill = rect.getMovedBy (m_offsets.top ()).getIntersection (m_clips.top ());

  if (!fill.isEmpty ())
  {
    auto left = fill.getLeft ();
    auto width = fill.getWidth ();
    auto bottom = fill.getBottom ();

    for (auto y = fill.getTop (); y <= bottom; y++)
      drawRawHorizontalLine (left, y, width);
  }
}

inline void FrameBuffer::drawRawHorizontalLine (tCoordinate x, tCoordinate y, tCoordinate length)
{
  auto fromIdx = getIndex (x, y);

#if _DEVELOPMENT_PC
  auto data = m_frontBuffer + fromIdx;
#else
  auto data = m_backBuffer.data () + fromIdx;
#endif

  for (long i = 0; i < length; i++)
    data[i] = m_currentColor;
}

void FrameBuffer::drawRect (tCoordinate x, tCoordinate y, tCoordinate width, tCoordinate height)
{
  drawRect (Rect (x, y, width, height));
}

void FrameBuffer::drawRect (const Rect &rect)
{
  if (!rect.isEmpty ())
  {
    drawHorizontalLine (rect.getLeft (), rect.getTop (), rect.getWidth ());
    drawHorizontalLine (rect.getLeft (), rect.getBottom (), rect.getWidth ());
    drawVerticalLine (rect.getLeft (), rect.getTop (), rect.getHeight ());
    drawVerticalLine (rect.getRight (), rect.getTop (), rect.getHeight ());
  }
}

void FrameBuffer::drawHorizontalLine (tCoordinate x, tCoordinate y, tCoordinate length)
{
  auto &clip = m_clips.top ();
  auto &offset = m_offsets.top ();

  x += offset.getX ();
  y += offset.getY ();

  if (y >= clip.getTop () && y <= clip.getBottom () && length > 0)
  {
    auto left = std::max (x, clip.getLeft ());
    auto right = std::min (x + length, clip.getLeft () + clip.getWidth ());
    drawRawHorizontalLine (left, y, right - left);
  }
}

void FrameBuffer::drawVerticalLine (tCoordinate x, tCoordinate y, tCoordinate length)
{
  auto &clip = m_clips.top ();
  auto &offset = m_offsets.top ();

  x += offset.getX ();
  y += offset.getY ();

  if (x >= clip.getLeft () && x <= clip.getRight () && length > 0)
  {
    auto top = std::max (y, clip.getTop ());
    auto bottom = std::min (y + length, clip.getTop () + clip.getHeight ());

    for (auto i = top; i < bottom; i++)
      setRawPixel (x, i);
  }
}

void FrameBuffer::swapBuffers ()
{
#if _DEVELOPMENT_PC
#else
  memcpy (m_frontBuffer, m_backBuffer.data (), m_buffersize);
#endif
}

FrameBuffer::Clip FrameBuffer::clip (const Rect &rect)
{
  return Clip (this, rect);
}

FrameBuffer::Clip FrameBuffer::clipRespectingOffset (const Rect &rect)
{
  return clip (rect.getMovedBy (m_offsets.top ()));
}

FrameBuffer::Offset FrameBuffer::offset (const Point &offset)
{
  return Offset (this, offset);
}
