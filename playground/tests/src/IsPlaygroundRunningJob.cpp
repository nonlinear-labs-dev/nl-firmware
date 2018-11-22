#include "IsPlaygroundRunningJob.h"

IsPlaygroundRunningJob::IsPlaygroundRunningJob(Job *parent)
    : super(parent)
{
}

IsPlaygroundRunningJob::~IsPlaygroundRunningJob()
{
}

void IsPlaygroundRunningJob::implRun()
{
  SoupMessage *msg = soup_message_new("GET", URL("/get-update?updateID=0"));
  soup_session_send_message(getSession(), msg);

  getParent()->onChildFinished(this, SOUP_STATUS_IS_SUCCESSFUL(msg->status_code));
  g_object_unref(msg);
}
