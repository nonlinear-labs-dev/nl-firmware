#include <io/ToLPCBridge.h>
#include "network/UDPReceiver.h"
#include "files/LPCSender.h"
#include "Application.h"
#include "Options.h"

ToLPCBridge::ToLPCBridge() :
    super(new LPCSender(), new UDPReceiver(Application::get().getOptions()->receiveMessagesForLpcOnPort()))
{
}

ToLPCBridge::~ToLPCBridge()
{
}

