#include <io/files/FileIOReceiver.h>
#include <io/FromButtonsBridge.h>
#include <io/network/WebSocketSender.h>

FromButtonsBridge::FromButtonsBridge() :
    Bridge(new WebSocketSender(Domain::Buttons),
           new FileIOReceiver("/dev/espi_buttons", 1))
{
}

FromButtonsBridge::~FromButtonsBridge()
{
}

