#pragma once

#include "bbbb.h"
#include <giomm/file.h>

class Options
{
  public:
    Options(int &argc, char **&argv);
    virtual ~Options();

    const char *getLPCSendFileName() const;
    const char *getLPCReceiveFileName() const;

    const char *getEncoderFileName() const;
    const char *getButtonsFileName() const;

    const char *getFrameBufferFileName() const;

    const char *forwardLPCMessagesTo() const;
    const char *forwardEncoderMessagesTo() const;
    const char *forwardButtonMessagesTo() const;

    int receiveMessagesForLpcOnPort() const;
    int receiveMessagesForOledsOnPort() const;



  private:
    bool setLPCSendFileName(const Glib::ustring &name);
    bool setLPCReceiveFileName(const Glib::ustring &name);

    Glib::ustring m_LPCSendFileName = "/dev/lpc_bb_driver";
    Glib::ustring m_LPCReceiveFileName = "/dev/lpc_bb_driver";
    Glib::ustring m_encoderFileName = "/dev/espi_encoder";
    Glib::ustring m_buttonsFileName = "/dev/espi_buttons";
};
