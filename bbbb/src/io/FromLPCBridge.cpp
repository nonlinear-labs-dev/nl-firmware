#include <io/FromLPCBridge.h>
#include "files/LPCReceiver.h"
#include "network/WebSocketSender.h"

FromLPCBridge::FromLPCBridge() :
    Bridge(new WebSocketSender(Domain::Lpc), new LPCReceiver())
{
}

FromLPCBridge::~FromLPCBridge()
{
}

