#include "UpdateService.h"
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <nltools/system/SpawnCommandLine.h>

UpdateService::UpdateService()
{
  nltools::msg::receive<nltools::msg::Update::UpdateUploadedNotification>(
      nltools::msg::EndPoint::BeagleBone, [this](auto) { onUpdateNotificationReceived(); });
}

void UpdateService::onUpdateNotificationReceived()
{
  SpawnCommandLine cmd("systemd-run /bin/sh /usr/C15/scripts/install-update-from-epc.sh");
}
