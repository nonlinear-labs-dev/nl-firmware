#include <io/framebuffer/FrameBufferSender.h>
#include <io/network/WebSocketReceiver.h>
#include <io/ToOledsBridge.h>

ToOledsBridge::ToOledsBridge() :
    super(new FrameBufferSender(), new WebSocketReceiver(Domain::Oled))
{
}

ToOledsBridge::~ToOledsBridge()
{
}

