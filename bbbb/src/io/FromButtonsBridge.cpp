#include <Application.h>
#include <io/files/FileIOReceiver.h>
#include <io/FromButtonsBridge.h>
#include <io/network/UDPSender.h>
#include <Options.h>

FromButtonsBridge::FromButtonsBridge() :
    Bridge(new UDPSender(Application::get().getOptions()->forwardButtonMessagesTo()),
           new FileIOReceiver(Application::get().getOptions()->getButtonsFileName(), 1))
{
}

FromButtonsBridge::~FromButtonsBridge()
{
}

