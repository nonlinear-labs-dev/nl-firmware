#pragma once

#include <io/files/FileIOSender.h>

class LPCSender : public FileIOSender
{
    using super = FileIOSender;

  public:
    LPCSender();
    virtual ~LPCSender();

};

