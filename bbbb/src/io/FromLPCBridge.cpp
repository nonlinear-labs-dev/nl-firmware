#include <io/FromLPCBridge.h>
#include "files/LPCReceiver.h"
#include "network/UDPSender.h"
#include "Application.h"
#include "Options.h"

FromLPCBridge::FromLPCBridge() :
    Bridge(new UDPSender(Application::get().getOptions()->forwardLPCMessagesTo()), new LPCReceiver())
{
}

FromLPCBridge::~FromLPCBridge()
{
}

