#include <io/ToRibbonLEDsBridge.h>
#include "network/WebSocketReceiver.h"
#include "files/FileIOSender.h"

ToRibbonLEDsBridge::ToRibbonLEDsBridge() :
    super(new FileIOSender("/dev/ribbon_led"), new WebSocketReceiver(Domain::RibbonLed))
{
}

ToRibbonLEDsBridge::~ToRibbonLEDsBridge()
{
}

