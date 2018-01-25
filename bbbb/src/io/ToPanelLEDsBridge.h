#pragma once

#include <io/Bridge.h>

class ToPanelLEDsBridge : public Bridge
{
    using super = Bridge;

  public:
    ToPanelLEDsBridge();
    virtual ~ToPanelLEDsBridge();

};

