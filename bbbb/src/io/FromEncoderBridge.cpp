#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>

FromEncoderBridge::FromEncoderBridge() :
    Bridge(new WebSocketSender(Domain::Rotary),
           new FileIOReceiver("/dev/espi_encoder", 1))
{
}

FromEncoderBridge::~FromEncoderBridge()
{
}

