#pragma once

#include "playground.h"
#include <vector>
#include <playcontroller/playcontroller-defs.h>
#include <playcontroller/playcontroller-converters.h>

class MessageParser
{
 public:
  MessageParser();
  virtual ~MessageParser();

  using MessageTypes = PLAYCONTROLLER_BB_MESSAGE_TYPES;
  using PlaycontrollerRequestTypes = PLAYCONTROLLER_REQUEST_IDS;

  struct NLMessage
  {
    NLMessage();
    ~NLMessage();

    uint16_t type;
    uint16_t length;
    std::vector<uint16_t> params;
  };

  [[nodiscard]] const NLMessage &getMessage() const;
  size_t parse(const uint8_t *buffer, size_t numBytes);
  static size_t getNumInitialBytesNeeded();

 private:
  [[nodiscard]] bool hasReadHeader() const;
  [[nodiscard]] size_t getNumHeaderBytesMissing() const;

  size_t parseBody(const uint8_t *buffer, size_t numBytes);

  NLMessage m_msg;
  size_t m_numBytesRead;
};
