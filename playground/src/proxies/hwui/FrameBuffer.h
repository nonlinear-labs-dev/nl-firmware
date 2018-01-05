#pragma once

#include <cstdint>
#include "controls/Rect.h"
#include <playground.h>
#include <linux/fb.h>
#include <tools/Uncopyable.h>

class Font;

class FrameBuffer : public Uncopyable
{
  public:
    virtual ~FrameBuffer ();

    static inline FrameBuffer &get ()
    {
      static FrameBuffer buf;
      return buf;
    }

    typedef int32_t tCoordinate;

#if _DEVELOPMENT_PC
    typedef uint32_t tPixel;

    template<uint32_t r, uint32_t g, uint32_t b>
    struct ToRGB
    {
      static constexpr uint32_t value = (0xFF << 24) | (r << 16) | (g << 8) | b;
    };

    enum Colors : tPixel
    {
      Undefined = 0,
      C43 = ToRGB<43, 32, 21>::value,
      C77 = ToRGB<77, 60, 10>::value,
      C103 = ToRGB<103, 81, 12>::value,
      C128 = ToRGB<128, 102, 16>::value,
      C179 = ToRGB<179, 142, 21>::value,
      C204 = ToRGB<204, 162, 24>::value,
      C255 = ToRGB<255, 203, 31>::value
    };

    tPixel interpolateColor(float normalized)
    {
      uint8_t c = normalized * 0xFF;
      return (0xFF << 24) | (c << 16) | (c << 8) | c;
    }

#else
    typedef uint8_t tPixel;

    enum Colors
      : tPixel
      {
        Undefined = 0xFF, C43 = 0x00, C77 = 0x02, C103 = 0x05, C128 = 0x06, C179 = 0x0A, C204 = 0x0B, C255 = 0x0F
    };

    tPixel interpolateColor (float normalized)
    {
      return normalized * 0x0F;
    }

#endif

    void setColor (const Colors &c);
    void fiddleColor (tPixel p);

    Colors getColor () const;

    void fillRect (tCoordinate left, tCoordinate top, tCoordinate width, tCoordinate height);
    void fillRect (const Rect &rect);

    void drawRect (tCoordinate left, tCoordinate top, tCoordinate width, tCoordinate height);
    void drawRect (const Rect &rect);

    void drawHorizontalLine (tCoordinate x, tCoordinate y, tCoordinate length);
    void drawVerticalLine (tCoordinate x, tCoordinate y, tCoordinate length);

    void setPixel (tCoordinate x, tCoordinate y);
    void clear ();

    struct StackScopeGuard
    {
        FrameBuffer *m_fb;

        StackScopeGuard (FrameBuffer *fb);
        StackScopeGuard (StackScopeGuard &&other);
        virtual ~StackScopeGuard ();
        StackScopeGuard (const Uncopyable &) = delete;
        StackScopeGuard &operator= (const Uncopyable &) = delete;
    };

    struct Clip : StackScopeGuard
    {
        Clip (FrameBuffer *fb, const Rect &clip);
        Clip (Clip &&other);
        ~Clip ();

        bool isEmpty () const;
    };

    struct Offset : StackScopeGuard
    {
        Offset (FrameBuffer *fb, const Point &offset);
        Offset (Offset &&other);
        ~Offset ();
    };

    friend struct Clip;
    friend struct Offset;

    Clip clip (const Rect &rect);
    Clip clipRespectingOffset (const Rect &rect);
    Offset offset (const Point &offset);

    void swapBuffers();

  private:
    FrameBuffer ();

    void setOffsetPixel (tCoordinate x, tCoordinate y);
    void setRawPixel (tCoordinate x, tCoordinate y);
    void drawRawHorizontalLine (tCoordinate x, tCoordinate y, tCoordinate length);

    void initStacks ();
    void openAndMap ();
    void unmapAndClose ();

    long getIndex(tCoordinate x, tCoordinate y) const;

    Colors m_currentColor = C43;
    int m_fd = -1;
    size_t m_buffersize = 0;

    tPixel *m_frontBuffer = nullptr;
    vector<tPixel> m_backBuffer;
    struct fb_var_screeninfo m_varInfo;

    std::stack<Rect> m_clips;
    std::stack<Point> m_offsets;
};

