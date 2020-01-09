#pragma once

#include "playground.h"
#include "NetworkRequest.h"
#include <set>
#include <memory>
#include <libsoup/soup-message.h>

class PendingHTTPRequests
{
 public:
  PendingHTTPRequests();
  virtual ~PendingHTTPRequests();

  typedef std::shared_ptr<NetworkRequest> tRequest;
  typedef std::set<tRequest> tMessageSet;
  void addPendingMessage(tRequest msg);
  tMessageSet expropriateFromPendingMessages();
  bool removeIfExists(SoupMessage *msg);

 private:
  tMessageSet m_pendingMessages;
};
