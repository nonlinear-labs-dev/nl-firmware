#include <io/ToPanelLEDsBridge.h>
#include "network/WebSocketReceiver.h"
#include "files/FileIOSender.h"

ToPanelLEDsBridge::ToPanelLEDsBridge()
    : super(new FileIOSender("/dev/espi_led"), new WebSocketReceiver(Domain::PanelLed))
{
}

ToPanelLEDsBridge::~ToPanelLEDsBridge()
{
}
