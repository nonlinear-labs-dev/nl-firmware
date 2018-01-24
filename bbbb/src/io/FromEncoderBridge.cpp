#include <Application.h>
#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/UDPSender.h>
#include <Options.h>

FromEncoderBridge::FromEncoderBridge() :
    Bridge(new UDPSender(Application::get().getOptions()->forwardEncoderMessagesTo()),
           new FileIOReceiver(Application::get().getOptions()->getEncoderFileName()))
{
}

FromEncoderBridge::~FromEncoderBridge()
{
}

