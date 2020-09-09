#pragma once

#include <http/ServedStream.h>

class ServedJournal : public ServedStream
{
 public:
  ServedJournal(HTTPServer &server, std::shared_ptr<HTTPRequest> request);
  virtual ~ServedJournal();

  void startServing() override;
};
