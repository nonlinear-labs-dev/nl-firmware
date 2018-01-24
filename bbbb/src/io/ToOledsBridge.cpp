#include <Application.h>
#include <io/framebuffer/FrameBufferSender.h>
#include <io/network/UDPReceiver.h>
#include <io/ToOledsBridge.h>
#include <Options.h>

ToOledsBridge::ToOledsBridge() :
    super(new FrameBufferSender(),
        new UDPReceiver(Application::get().getOptions()->receiveMessagesForOledsOnPort()))
{
}

ToOledsBridge::~ToOledsBridge()
{
}

