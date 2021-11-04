#include "EditBufferMessageComposer.h"
#include "MessageParser.h"

EditBufferMessageComposer::EditBufferMessageComposer()
    : super(MessageParser::MessageTypes::PLAYCONTROLLER_BB_MSG_TYPE_PRESET_DIRECT)
{
}
